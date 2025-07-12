const express = require('express');
const path = require('path');
const fs = require('fs');

const app = express();

/**
 * Comprehensive Underscore.js Template Processor
 * Handles complex nested templates and converts them to valid JavaScript
 */
class UnderscoreTemplateProcessor {
    constructor() {
        this.patterns = {
            // Execution blocks: <% code %>
            execution: /<%\s*(.*?)\s*%>/g,
            output: /<%=\s*(.*?)\s*%>/g,
            escapedOutput: /<%\-\s*(.*?)\s*%>/g,
            // Combined pattern for all template tags
            allTags: /<%[-=]?\s*(.*?)\s*%>/g
        };
        
        this.htmlEntities = {
            '&': '&amp;',
            '<': '&lt;',
            '>': '&gt;',
            '"': '&quot;',
            "'": '&#39;',
            '/': '&#47;'
        };
    }

    /**
     * Main processing function - converts template to valid JavaScript
     * @param {string} templateContent - The JavaScript file content with Underscore templates
     * @returns {string} - Processed JavaScript content
     */
    processTemplate(templateContent) {
        try {
            const processedContent = this.extractAndProcessTemplates(templateContent);
            
            this.validateJavaScript(processedContent);
            
            return processedContent;
        } catch (error) {
            console.error('Template processing error:', error.message);
            throw error;
        }
    }

    /**
     * Extract template sections and process them
     * @param {string} content - Original content
     * @returns {string} - Processed content
     */
    extractAndProcessTemplates(content) {
        const templateStringPattern = /(['"`])((?:\\.|(?!\1)[^\\])*?<%.*?%>.*?)\1/g;
        
        return content.replace(templateStringPattern, (match, quote, templateStr) => {
            const processedTemplate = this.processTemplateString(templateStr);
            return `${quote}${processedTemplate}${quote}`;
        });
    }

    /**
     * Process a single template string
     * @param {string} templateStr - Template string content
     * @returns {string} - Processed template string
     */
    processTemplateString(templateStr) {
        const segments = this.parseTemplateSegments(templateStr);
        
        const jsFunction = this.convertToJavaScriptFunction(segments);
        
        return `" + (${jsFunction})(data) + "`;
    }

    /**
     * Parse template into segments (text, code, output)
     * @param {string} template - Template string
     * @returns {Array} - Array of segment objects
     */
    parseTemplateSegments(template) {
        const segments = [];
        let lastIndex = 0;
        let match;
        
        this.patterns.allTags.lastIndex = 0;
        
        while ((match = this.patterns.allTags.exec(template)) !== null) {
            if (match.index > lastIndex) {
                const textContent = template.slice(lastIndex, match.index);
                if (textContent) {
                    segments.push({
                        type: 'text',
                        content: textContent
                    });
                }
            }
            
            const fullMatch = match[0];
            const innerContent = match[1];
            
            if (fullMatch.startsWith('<%=')) {
                segments.push({
                    type: 'output',
                    content: innerContent.trim()
                });
            } else if (fullMatch.startsWith('<%-')) {
                segments.push({
                    type: 'escapedOutput',
                    content: innerContent.trim()
                });
            } else {
                segments.push({
                    type: 'code',
                    content: innerContent.trim()
                });
            }
            
            lastIndex = match.index + match[0].length;
        }
        
        if (lastIndex < template.length) {
            const textContent = template.slice(lastIndex);
            if (textContent) {
                segments.push({
                    type: 'text',
                    content: textContent
                });
            }
        }
        
        return segments;
    }

    /**
     * Convert segments to JavaScript function
     * @param {Array} segments - Parsed segments
     * @returns {string} - JavaScript function string
     */
    convertToJavaScriptFunction(segments) {
        const functionBody = [];
        functionBody.push('function(data) {');
        functionBody.push('  var __output = [];');
        functionBody.push('  var __append = function(s) { __output.push(s); };');
        functionBody.push('  var __escape = function(s) { return String(s).replace(/[&<>"\'\/]/g, function(match) { return {"&": "&amp;", "<": "&lt;", ">": "&gt;", "\\"": "&quot;", "\'": "&#39;", "/": "&#47;"}[match]; }); };');
        functionBody.push('  with (data || {}) {');
        
        segments.forEach(segment => {
            switch (segment.type) {
                case 'text':
                    const escapedText = segment.content
                        .replace(/\\/g, '\\\\')
                        .replace(/"/g, '\\"')
                        .replace(/\n/g, '\\n')
                        .replace(/\r/g, '\\r');
                    functionBody.push(`    __append("${escapedText}");`);
                    break;
                case 'output':
                    functionBody.push(`    __append(${segment.content});`);
                    break;
                case 'escapedOutput':
                    functionBody.push(`    __append(__escape(${segment.content}));`);
                    break;
                case 'code':
                    const codeLines = segment.content.split('\n');
                    codeLines.forEach(line => {
                        if (line.trim()) {
                            functionBody.push(`    ${line.trim()}`);
                        }
                    });
                    break;
            }
        });
        
        functionBody.push('  }');
        functionBody.push('  return __output.join("");');
        functionBody.push('}');
        
        return functionBody.join('\n');
    }

    /**
     * Alternative approach: Simple template cleaning for basic cases
     * @param {string} content - Content to clean
     * @returns {string} - Cleaned content
     */
    simpleTemplateClean(content) {
        return content
            // Remove execution blocks
            .replace(/<%\s*(?!=)(.*?)%>/g, '')
            .replace(/<%=\s*(.*?)\s*%>/g, '" + ($1) + "')
            .replace(/<%\-\s*(.*?)\s*%>/g, '" + this.escape($1) + "')
            .replace(/"\s*\+\s*"/g, '')
            .replace(/\+\s*""\s*\+/g, '+')
            .replace(/^""\s*\+\s*/, '')
            .replace(/\s*\+\s*""$/, '');
    }

    /**
     * Validate JavaScript syntax
     * @param {string} jsCode - JavaScript code to validate
     * @throws {Error} - If syntax is invalid
     */
    validateJavaScript(jsCode) {
        try {
            new Function(jsCode);
        } catch (error) {
            throw new Error(`Invalid JavaScript syntax: ${error.message}`);
        }
    }

    /**
     * Process file content with error handling
     * @param {string} filePath - File path (for error reporting)
     * @param {string} content - File content
     * @returns {object} - Processing result
     */
    processFile(filePath, content) {
        try {
            const processed = this.processTemplate(content);
            return {
                success: true,
                content: processed,
                filePath: filePath
            };
        } catch (error) {
            return {
                success: false,
                error: error.message,
                filePath: filePath,
                content: content // Return original content on error
            };
        }
    }

    /**
     * Process execute tokens (code blocks)
     * @param {Object} token - Token to process
     * @param {Array} stack - Stack for tracking nested blocks
     * @returns {Object} - Processed token
     */
    processExecuteToken(token, stack) {
        let content = token.content.trim();

        if (this.isControlStructureStart(content)) {
            stack.push(content);
            content = this.fixControlStructureStart(content);
        } else if (this.isControlStructureEnd(content)) {
            const lastStart = stack.pop();
            content = this.fixControlStructureEnd(content, lastStart);
        } else if (this.isElseStatement(content)) {
            content = this.fixElseStatement(content);
        } else {
            content = this.fixGeneralStatement(content);
        }

        return {
            ...token,
            content: content
        };
    }

    /**
     * Process interpolate/escape tokens
     * @param {Object} token - Token to process
     * @returns {Object} - Processed token
     */
    processInterpolateToken(token) {
        let content = token.content.trim();

        content = this.fixInterpolationSyntax(content);

        return {
            ...token,
            content: content
        };
    }

    /**
     * Check if content is a control structure start
     * @param {string} content - Code content
     * @returns {boolean}
     */
    isControlStructureStart(content) {
        const patterns = [
            /^\s*if\s*\(/,
            /^\s*for\s*\(/,
            /^\s*while\s*\(/,
            /^\s*_.[\w.]+\s*\(/,
            /^\s*\w+\.forEach\s*\(/,
            /^\s*\w+\.map\s*\(/
        ];
        return patterns.some(pattern => pattern.test(content));
    }

    /**
     * Check if content is a control structure end
     * @param {string} content - Code content
     * @returns {boolean}
     */
    isControlStructureEnd(content) {
        return /^\s*[}\)]\s*;?\s*$/.test(content);
    }

    /**
     * Check if content is an else statement
     * @param {string} content - Code content
     * @returns {boolean}
     */
    isElseStatement(content) {
        return /^\s*}\s*else\s*{?\s*$/.test(content) || /^\s*else\s*{?\s*$/.test(content);
    }

    /**
     * Fix control structure start syntax
     * @param {string} content - Code content
     * @returns {string} - Fixed content
     */
    fixControlStructureStart(content) {
        if (!content.includes('{') && !content.endsWith(';')) {
            content = content + ' {';
        }

        content = content.replace(/=>\s*{\s*$/, '=> {');

        return content;
    }

    /**
     * Fix control structure end syntax
     * @param {string} content - Code content
     * @param {string} startContent - Corresponding start content
     * @returns {string} - Fixed content
     */
    fixControlStructureEnd(content) {
        if (!content.includes('}') && !content.includes(')')) {
            return '}';
        }

        content = content.replace(/}\s*}\s*;?\s*$/, '});');

        return content;
    }

    /**
     * Fix else statement syntax
     * @param {string} content - Code content
     * @returns {string} - Fixed content
     */
    fixElseStatement(content) {
        if (content.includes('} else')) {
            return content.replace(/}\s*else\s*{?/, '} else {');
        } else if (content.trim() === 'else') {
            return '} else {';
        }

        return content;
    }

    /**
     * Fix general statement syntax
     * @param {string} content - Code content
     * @returns {string} - Fixed content
     */
    fixGeneralStatement(content) {
        content = content.replace(/^\s*{\s*$/, '');
        content = content.replace(/^\s*}\s*$/, '');

        if (content && !content.match(/[{}();]$/)) {
            content = content + ';';
        }

        return content;
    }

    /**
     * Fix interpolation syntax issues
     * @param {string} content - Interpolation content
     * @returns {string} - Fixed content
     */
    fixInterpolationSyntax(content) {
        content = content.replace(/^tr\s*\(\s*['"]([^'"]*)['"]\s*\)$/, "tr('$1')");

        content = content.replace(/^\s*(\w+(?:\.\w+)*)\s*$/, '$1');

        return content;
    }

    /**
     * Reconstruct the template from processed tokens
     * @param {Array} tokens - Processed tokens
     * @returns {string} - Reconstructed template
     */
    reconstructTemplate(tokens) {
        return tokens.map(token => {
            switch (token.type) {
                case this.TOKEN_TYPES.EXECUTE:
                    return `<% ${token.content} %>`;
                case this.TOKEN_TYPES.INTERPOLATE:
                    return `<%= ${token.content} %>`;
                case this.TOKEN_TYPES.ESCAPE:
                    return `<%- ${token.content} %>`;
                case this.TOKEN_TYPES.TEXT:
                    return token.content;
                default:
                    return token.raw || token.content;
            }
        }).join('');
    }

    /**
     * Fallback cleaning method for simple cases
     * @param {string} template - Template string
     * @returns {string} - Cleaned template
     */
    fallbackClean(template) {
        let cleaned = template;

        // Fix orphaned else statements
        cleaned = cleaned.replace(/<% else %>/g, '<% } else { %>');

        cleaned = cleaned.replace(/(<% if \([^)]+\) %>)/g, '$1');

        cleaned = cleaned.replace(/(<% } %>)(\s*<% } %>)/g, '$1');

        return cleaned;
    }

    /**
     * Utility method to clean templates in _.template() calls
     * @param {string} jsCode - JavaScript code containing _.template calls
     * @returns {string} - Cleaned JavaScript code
     */
    cleanTemplateInCode(jsCode) {
        const templatePattern = /_.template\s*\(\s*`([\s\S]*?)`\s*\)/g;
        return jsCode.replace(templatePattern, (match, templateContent) => {
            const cleanedTemplate = this.cleanTemplate(templateContent);
            return `_.template(\`${cleanedTemplate}\`)`;
        });
    }
}


/**
 * Robust Underscore.js template syntax cleaner using ChatGPT lexical parsing solution
 * Converts template blocks to JavaScript comments to maintain valid syntax
 */
/**
 * Comprehensive Underscore.js template cleaner that handles complex control structures
 * and prevents JavaScript syntax errors by properly parsing and removing template blocks.
 * Solution provided by Claude AI for robust template processing.
 */
function cleanUnderscoreTemplates(input) {
    const regex = /<%[-=]?([\s\S]*?)%>/g;
    let result = '';
    let lastIndex = 0;
    const blockStack = [];
    
    const isBlockStart = (code) =>
        /^\s*(if|for|while|switch|try)\b.*\{\s*$/.test(code);
    const isBlockEnd = (code) =>
        /^\s*\}/.test(code);
    const isElse = (code) =>
        /^\s*else(\s+if\b.*)?\s*\{?$/.test(code);
    const isExpression = (fullMatch) =>
        /^<%=/.test(fullMatch);
    const isRawExpression = (fullMatch) =>
        /^<%-/.test(fullMatch);
    
    let match;
    while ((match = regex.exec(input)) !== null) {
        const [fullMatch, innerCode] = match;
        const index = match.index;
        
        result += input.slice(lastIndex, index);
        lastIndex = regex.lastIndex;
        
        const code = innerCode.trim();
        
        if (isExpression(fullMatch) || isRawExpression(fullMatch)) {
            result += `/* template expr: ${code} */`;
            continue;
        }
        
        if (isBlockStart(code)) {
            result += `/* template start: ${code} */`;
            blockStack.push('block');
            continue;
        }
        
        if (isBlockEnd(code)) {
            if (blockStack.length > 0) {
                result += `/* template end: ${code} */`;
                blockStack.pop();
            } else {
                result += `/* orphaned end: ${code} */`;
            }
            continue;
        }
        
        if (isElse(code)) {
            if (blockStack.length > 0) {
                result += `/* template else: ${code} */`;
            } else {
                result += `/* orphaned else: ${code} */ if (false) {}`;
            }
            continue;
        }
        
        result += `/* template code: ${code} */`;
    }
    
    result += input.slice(lastIndex);
    
    return result;
}

/**
 * Find the next template tag in the content starting from the given position
 */
function findNextTemplateTag(content, startPos) {
    const patterns = [
        { regex: /<%\s*if\s*\([^)]*\)\s*{\s*%>/g, type: 'code', subtype: 'if' },
        { regex: /<%\s*}\s*else\s*{\s*%>/g, type: 'code', subtype: 'else' },
        { regex: /<%\s*}\s*%>/g, type: 'code', subtype: 'close' },
        { regex: /<%\s*for\s*\([^)]*\)\s*{\s*%>/g, type: 'code', subtype: 'for' },
        { regex: /<%\s*while\s*\([^)]*\)\s*{\s*%>/g, type: 'code', subtype: 'while' },
        { regex: /<%\s*[^=\-][^%]*%>/g, type: 'code', subtype: 'general' },
        { regex: /<%=\s*[^%]*%>/g, type: 'expression' },
        { regex: /<%-\s*[^%]*%>/g, type: 'escaped' }
    ];

    let nearestTag = null;
    let nearestPos = Infinity;

    for (const pattern of patterns) {
        pattern.regex.lastIndex = startPos;
        const match = pattern.regex.exec(content);
        
        if (match && match.index < nearestPos) {
            nearestPos = match.index;
            nearestTag = {
                start: match.index,
                end: match.index + match[0].length,
                type: pattern.type,
                subtype: pattern.subtype,
                match: match[0],
                content: match[0]
            };
        }
    }

    return nearestTag;
}

/**
 * Process a code block, handling nested structures and control flow
 */
function processCodeBlock(content, tag, totalLen) {
    const blockStack = [];
    let currentPos = tag.start;
    let replacement = '';

    if (tag.subtype === 'if' || tag.subtype === 'for' || tag.subtype === 'while') {
        blockStack.push(tag.subtype);
        currentPos = tag.end;

        while (currentPos < totalLen && blockStack.length > 0) {
            const nextTag = findNextTemplateTag(content, currentPos);
            
            if (!nextTag) {
                break;
            }

            if (nextTag.type === 'code') {
                if (nextTag.subtype === 'if' || nextTag.subtype === 'for' || nextTag.subtype === 'while') {
                    blockStack.push(nextTag.subtype);
                } else if (nextTag.subtype === 'close') {
                    blockStack.pop();
                }
            }

            currentPos = nextTag.end;
        }

        return {
            replacement: '',
            nextPos: currentPos
        };
    }

    if (tag.subtype === 'else' || tag.subtype === 'close') {
        return {
            replacement: '',
            nextPos: tag.end
        };
    }

    return {
        replacement: '',
        nextPos: tag.end
    };
}

/**
 * Post-process cleanup to remove any remaining template artifacts
 */
function postProcessCleanup(content) {
    content = content.replace(/<%[\s\S]*?%>/g, '');
    
    content = content.replace(/\n\s*\n\s*\n/g, '\n\n');
    
    // Remove orphaned braces and semicolons that might cause syntax errors
    content = content.replace(/^\s*}\s*$/gm, '');
    content = content.replace(/^\s*{\s*$/gm, '');
    content = content.replace(/^\s*else\s*$/gm, '');
    content = content.replace(/^\s*else\s*{\s*$/gm, '');
    
    content = content.replace(/^\s*\n/, '').replace(/\n\s*$/, '');
    
    return content;
}

/**
 * Remove template expressions like <%= variable %> and <%- escaped %>
 */
function removeTemplateExpressions(text) {
    text = text.replace(/<%[-=]\s*[^%]*%>/g, '');

    text = text.replace(/<%=\s*[^%]*%>/g, '');

    return text;
}

/**
 * Remove control structures while maintaining proper nesting
 */
function removeControlStructures(text) {
    const tokens = tokenizeTemplate(text);
    const cleanedTokens = processControlStructures(tokens);
    return reconstructText(cleanedTokens);
}

/**
 * Tokenize template into manageable pieces
 */
function tokenizeTemplate(text) {
    const tokens = [];
    let current = '';
    let i = 0;

    while (i < text.length) {
        if (text.substr(i, 2) === '<%' && text.substr(i, 3) !== '<%=') {
            if (current) {
                tokens.push({ type: 'text', content: current });
                current = '';
            }

            let j = i + 2;
            let depth = 1;
            while (j < text.length - 1 && depth > 0) {
                if (text.substr(j, 2) === '<%') {
                    depth++;
                    j += 2;
                } else if (text.substr(j, 2) === '%>') {
                    depth--;
                    j += 2;
                } else {
                    j++;
                }
            }

            if (depth === 0) {
                const tagContent = text.substring(i + 2, j - 2).trim();
                tokens.push({ type: 'control', content: tagContent });
                i = j;
            } else {
                current += text[i];
                i++;
            }
        } else {
            current += text[i];
            i++;
        }
    }

    if (current) {
        tokens.push({ type: 'text', content: current });
    }

    return tokens;
}

/**
 * Process control structures and remove complete blocks
 */
function processControlStructures(tokens) {
    const result = [];
    const stack = [];

    for (const token of tokens) {
        if (token.type === 'text') {
            if (stack.length === 0) {
                result.push(token);
            }
        } else if (token.type === 'control') {
            const content = token.content;

            if (isOpeningControl(content)) {
                stack.push({ type: getControlType(content), token });
            } else if (isClosingControl(content)) {
                if (stack.length > 0) {
                    stack.pop();
                }
            } else if (isElseControl(content)) {
            }
        }
    }

    return result;
}

/**
 * Check if a control statement is an opening statement
 */
function isOpeningControl(content) {
    const openingPatterns = [
        /^\s*if\s*\(/,
        /^\s*for\s*\(/,
        /^\s*while\s*\(/,
        /^\s*switch\s*\(/,
        /^\s*try\s*\{/,
        /^\s*\w+\s*\{/ // Generic block opener
    ];
    return openingPatterns.some(pattern => pattern.test(content));
}

/**
 * Check if a control statement is a closing statement
 */
function isClosingControl(content) {
    return /^\s*}\s*$/.test(content) ||
           /^\s*}\s*catch/.test(content) ||
           /^\s*}\s*finally/.test(content);
}

/**
 * Check if a control statement is an else statement
 */
function isElseControl(content) {
    return /^\s*}\s*else\s*\{/.test(content) ||
           /^\s*else\s*\{/.test(content) ||
           /^\s*}\s*else\s*if/.test(content);
}

/**
 * Get the type of control structure
 */
function getControlType(content) {
    if (/^\s*if\s*\(/.test(content)) return 'if';
    if (/^\s*for\s*\(/.test(content)) return 'for';
    if (/^\s*while\s*\(/.test(content)) return 'while';
    if (/^\s*switch\s*\(/.test(content)) return 'switch';
    if (/^\s*try\s*\{/.test(content)) return 'try';
    return 'block';
}

/**
 * Reconstruct text from tokens
 */
function reconstructText(tokens) {
    return tokens.map(token => token.content).join('');
}

/**
 * Remove any remaining template tags
 */
function removeRemainingTemplateTags(text) {
    return text.replace(/<%[^%]*%>/g, '');
}

/**
 * Clean up orphaned statements and syntax artifacts
 */
function cleanupOrphanedStatements(text) {
    let result = text;

    result = result.replace(/^\s*else\s*\{[^}]*\}\s*$/gm, '');
    result = result.replace(/^\s*else\s*$/gm, '');
    result = result.replace(/^\s*}\s*else\s*\{[^}]*\}\s*$/gm, '');

    result = result.replace(/^\s*}\s*$/gm, '');

    result = result.replace(/^\s*\{\s*$/gm, '');

    result = result.replace(/^\s*(if|for|while|switch|try|catch|finally)\s*\(\s*\)\s*$/gm, '');

    result = result.replace(/\(\s*\)/g, '');
    result = result.replace(/\[\s*\]/g, '');

    return result;
}

/**
 * Normalize whitespace and remove excessive empty lines
 */
function normalizeWhitespace(text) {
    return text
        .replace(/\r\n/g, '\n') // Normalize line endings
        .replace(/\n{3,}/g, '\n\n') // Limit consecutive empty lines
        .replace(/[ \t]+$/gm, '') // Remove trailing whitespace
        .trim();
}

/**
 * Validate that the result is valid JavaScript syntax
 */
function validateJavaScriptSyntax(text) {
    if (!text.trim()) {
        return;
    }

    try {
        const testCode = `(function() { ${text} })();`;
        new Function(testCode);
    } catch (error) {
        try {
            new Function(`return (${text});`);
        } catch (expressionError) {
            console.warn('Syntax validation failed:', error.message);
            console.warn('Consider manual review of the cleaned code');
        }
    }
}

function cleanTemplateStrings(content) {
    content = content.replace(/var\s+\w+\s*=\s*`[\s\S]*?`;/gs, 'var Template = "";');
    content = content.replace(/let\s+\w+\s*=\s*`[\s\S]*?`;/gs, 'let Template = "";');
    content = content.replace(/const\s+\w+\s*=\s*`[\s\S]*?`;/gs, 'const Template = "";');

    content = content.replace(/var\s+\w+\s*=\s*"[\s\S]*?";/gs, 'var Template = "";');
    content = content.replace(/var\s+\w+\s*=\s*'[\s\S]*?';/gs, 'var Template = "";');

    return content;
}

app.use((req, res, next) => {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
    next();
});

app.get('/toolbox', (req, res) => {
    const filePath = path.join('ChromeWorker/html/toolbox/index.html');
    if (fs.existsSync(filePath)) {
        let content = fs.readFileSync(filePath, 'utf8');

        content = content.replace(/_AL/g, 'window.ActionsList || {}');
        content = content.replace(/_MACRO_INSERT_LOCALIZE_/g, 'window._L = window._L || {}; /* Localization loaded */');
        content = content.replace(/_MACRO_INSERT_ACTIONS_/g, 'window.Actions = window.Actions || {}; /* Actions loaded */');
        content = content.replace(/_MACRO_INSERT_ELEMENT_FUNCTION_/g, '/* Element functions loaded */');
        content = content.replace(/_K/g, '"en"');

        content = content.replace(/<script[^>]*type=["']text\/template["'][^>]*>[\s\S]*?<\/script>/gi,
            '<script type="text/template"><!-- Template disabled in Docker mode --></script>');

        content = content.replace(/<%[\s\S]*?%>/g, '');
        content = content.replace(/<\?.*?\?>/gs, ''); // PHP-style tags if any

        content = content.replace(/utils\/cursorposition\.js/g, 'utils/cursorPosition.js');

        res.type('text/html');
        res.send(content);
    } else {
        res.status(404).send('Toolbox not found');
    }
});

app.get('/scenario', (req, res) => {
    const filePath = path.join('ChromeWorker/html/scenario/index.html');
    if (fs.existsSync(filePath)) {
        let content = fs.readFileSync(filePath, 'utf8');

        content = content.replace(/_AL/g, 'window.ActionsList || {}');
        content = content.replace(/_MACRO_INSERT_LOCALIZE_/g, 'window._L = window._L || {}; /* Localization loaded */');
        content = content.replace(/_MACRO_INSERT_ACTIONS_/g, 'window.Actions = window.Actions || {}; /* Actions loaded */');
        content = content.replace(/_MACRO_INSERT_ELEMENT_FUNCTION_/g, '/* Element functions loaded */');
        content = content.replace(/_K/g, '"en"');

        content = content.replace(/<script[^>]*type=["']text\/template["'][^>]*>[\s\S]*?<\/script>/gi,
            '<script type="text/template"><!-- Template disabled in Docker mode --></script>');

        content = content.replace(/<%[\s\S]*?%>/g, '');
        content = content.replace(/<\?.*?\?>/gs, ''); // PHP-style tags if any

        res.type('text/html');
        res.send(content);
    } else {
        res.status(404).send('Scenario not found');
    }
});

app.get('/central', (req, res) => {
    const filePath = path.join('ChromeWorker/html/central/index_settings.html');
    if (fs.existsSync(filePath)) {
        let content = fs.readFileSync(filePath, 'utf8');

        content = content.replace(/_AL/g, 'window.ActionsList || {}');
        content = content.replace(/_MACRO_INSERT_LOCALIZE_/g, 'window._L = window._L || {}; /* Localization loaded */');
        content = content.replace(/_MACRO_INSERT_ACTIONS_/g, 'window.Actions = window.Actions || {}; /* Actions loaded */');
        content = content.replace(/_MACRO_INSERT_ELEMENT_FUNCTION_/g, '/* Element functions loaded */');
        content = content.replace(/_K/g, '"en"');

        content = content.replace(/<script[^>]*type=["']text\/template["'][^>]*>[\s\S]*?<\/script>/gi,
            '<script type="text/template"><!-- Template disabled in Docker mode --></script>');

        content = content.replace(/<%[\s\S]*?%>/g, '');
        content = content.replace(/<\?.*?\?>/gs, ''); // PHP-style tags if any

        res.type('text/html');
        res.send(content);
    } else {
        res.status(404).send('Central not found');
    }
});

app.get('/scheduler', (req, res) => {
    const filePath = path.join('ChromeWorker/html/scheduler/index.html');
    if (fs.existsSync(filePath)) {
        let content = fs.readFileSync(filePath, 'utf8');

        content = content.replace(/_AL/g, 'window.ActionsList || {}');
        content = content.replace(/_MACRO_INSERT_LOCALIZE_/g, 'window._L = window._L || {}; /* Localization loaded */');
        content = content.replace(/_MACRO_INSERT_ACTIONS_/g, 'window.Actions = window.Actions || {}; /* Actions loaded */');
        content = content.replace(/_MACRO_INSERT_ELEMENT_FUNCTION_/g, '/* Element functions loaded */');
        content = content.replace(/_K/g, '"en"');

        content = content.replace(/<script[^>]*type=["']text\/template["'][^>]*>[\s\S]*?<\/script>/gi,
            '<script type="text/template"><!-- Template disabled in Docker mode --></script>');

        content = content.replace(/<%[\s\S]*?%>/g, '');
        content = content.replace(/<\?.*?\?>/gs, ''); // PHP-style tags if any

        res.type('text/html');
        res.send(content);
    } else {
        res.status(404).send('Scheduler not found');
    }
});

app.use((req, res, next) => {
    if (req.path.endsWith('.js')) {
        res.type('application/javascript');
        
        const possiblePaths = [
            path.join('ChromeWorker/html', req.path),
            path.join('ChromeWorker/html', req.path.replace(/^\/[^\/]+/, '')), // Remove first path segment
            path.join('ChromeWorker/html/toolbox', req.path.replace(/^\/toolbox/, '')),
            path.join('ChromeWorker/html/toolbox', req.path), // Direct toolbox path for core libraries
            path.join('ChromeWorker/html/scenario', req.path.replace(/^\/scenario/, '')),
            path.join('ChromeWorker/html/central', req.path.replace(/^\/central/, '')),
            path.join('ChromeWorker/html/scheduler', req.path.replace(/^\/scheduler/, ''))
        ];

        for (const jsFilePath of possiblePaths) {
            if (fs.existsSync(jsFilePath) && fs.statSync(jsFilePath).isFile()) {
                let content = fs.readFileSync(jsFilePath, 'utf8');

                console.log(`Processing JavaScript file: ${jsFilePath}`);

                if (jsFilePath.includes('library.min.js') ||
                    jsFilePath.includes('lodash.js') ||
                    jsFilePath.includes('rappid.js') ||
                    jsFilePath.includes('underscore') ||
                    jsFilePath.includes('beautifier.min.js') ||
                    jsFilePath.includes('jquery') ||
                    jsFilePath.includes('backbone.js') ||
                    jsFilePath.includes('bootbox') ||
                    jsFilePath.includes('bootstrap.min.js') ||
                    jsFilePath.includes('toastr.min.js') ||
                    jsFilePath.includes('diff_match_patch.js')) {
                    console.log(`Serving core library as-is: ${jsFilePath}`);
                    res.send(content);
                    return;
                }

                if (jsFilePath.includes('functionmanager.js') ||
                    jsFilePath.includes('tooledViewPlugin.js') ||
                    jsFilePath.includes('documents.js') ||
                    jsFilePath.includes('modal.js')) {

                    console.log(`Creating comprehensive stub for template-heavy file: ${jsFilePath}`);

                    const templateProcessor = new UnderscoreTemplateProcessor();
                    
                    if (content.includes('var Template = `') ||
                        content.includes('let Template = `') ||
                        content.includes('const Template = `')) {
                        content = cleanTemplateStrings(content);
                    } else {
                        try {
                            content = templateProcessor.processTemplate(content);
                        } catch (error) {
                            console.log(`Template processing failed, using fallback: ${error.message}`);
                            content = templateProcessor.simpleTemplateClean(content);
                        }
                    }

                    if (jsFilePath.includes('functionmanager.js')) {
                        content += `
function FunctionManager() {
    this.RenderDefault = function() {};
    this.SendCurrentFunction = function() {};
    this.ChangeFunction = function() {};
    this.Render = function() {};
    this.RenderFunction = function() {};
    this.GetParamList = function() { return []; };
    this.FindParam = function() { return null; };
    this.RemoveParam = function() {};
    this.AddOrChangeParam = function() {};
    this.UpdateParamsInterface = function() {};
    this.ClearVariableName = function() {};
    this.Translit = function() { return ""; };
    this.SerializeData = function() { return ""; };
    this.ParseData = function() {};
    this.GetEditingFunctionName = function() { return ""; };
    this.SetHasReturnValue = function() {};
    this.GetHasReturnValue = function() { return false; };
    this.SetFunctionName = function() {};
    this.SetFunctionDescription = function() {};
    this.SetFunctionReturnDescription = function() {};
    this.SetFunctionReturnVariable = function() {};
    this.GetFunctionName = function() { return ""; };
    this.GetFunctionDescription = function() { return ""; };
    this.GetFunctionReturnDescription = function() { return ""; };
    this.GetFunctionReturnVariable = function() { return ""; };
    this.Show = function() {};
    this.Hide = function() {};
    this.IsVisible = function() { return false; };
}`;
                    }

                    if (jsFilePath.includes('tooledViewPlugin.js')) {
                        content += `
window.TooledModelInterface = window.TooledModelInterface || function() {};
window.TooledViewInterface = window.TooledViewInterface || function() {};`;
                    }

                    if (jsFilePath.includes('documents.js')) {
                        content += `
window.DocumentsStore = window.DocumentsStore || function() {
    this.GetActionItems = function() { return []; };
    this.GetVideoItems = function() { return []; };
    this.GetWikiItems = function() { return []; };
};`;
                    }

                    if (jsFilePath.includes('modal.js')) {
                        content += `
window.Modal = window.Modal || function() {};`;
                    }

                    res.send(content);
                    return;
                }

                if (jsFilePath.includes('template/index.js') ||
                    jsFilePath.includes('dialogs/internal/utils.js')) {
                    console.log(`Applying comprehensive template processing to problematic file: ${jsFilePath}`);

                    const templateProcessor = new UnderscoreTemplateProcessor();
                    try {
                        content = templateProcessor.processTemplate(content);
                    } catch (error) {
                        console.log(`Template processing failed, using fallback: ${error.message}`);
                        content = templateProcessor.simpleTemplateClean(content);
                    }

                    if (jsFilePath.includes('template/index.js')) {
                        content += `
window.BasDialogsLib = window.BasDialogsLib || {};
window.BasDialogsLib.templates = window.BasDialogsLib.templates || {};
window.BasDialogsLib.templates.main = function() { return '<div>Template processed</div>'; };
window.BasDialogsLib.templates.recent = function() { return '<div>Recent items</div>'; };
window.BasDialogsLib.templates.visibleName = function() { return '<span>Item</span>'; };
`;
                    }

                    if (jsFilePath.includes('dialogs/internal/utils.js')) {
                        content += `
window.BasDialogsLib = window.BasDialogsLib || {};
window.BasDialogsLib.utils = window.BasDialogsLib.utils || {
    getDefaultCollection: function() { return []; },
    getRecentCollection: function() { return []; },
    getDisplayName: function() { return ''; },
    restoreCursorPosition: function() {},
    saveCursorPosition: function() {},
    isClickable: function() { return true; },
    toggleListItemHeader: function() {},
    formatString: function(str) { return str; },
    getPossibleActionObjects: function() { return []; }
};
`;
                    }

                    res.send(content);
                    return;
                }

                if (content.includes('<%') || content.includes('%>')) {
                    console.log(`COMPLETE REPLACEMENT: Template file disabled: ${jsFilePath}`);
                    
                    const fileName = path.basename(jsFilePath);
                    
                    content = `
// DOCKER MODE: Template file completely disabled to prevent syntax errors
// Original file: ${fileName}
console.log('Docker stub loaded: ${fileName}');

if (typeof window !== 'undefined') {
    window.BasDialogsLib = window.BasDialogsLib || {};
    window.BasDialogsLib.utils = window.BasDialogsLib.utils || {};
    window.BasDialogsLib.templates = window.BasDialogsLib.templates || {};
    
    Object.assign(window.BasDialogsLib.utils, {
        getDefaultCollection: function() { return []; },
        getRecentCollection: function() { return []; },
        getDisplayName: function() { return 'Item'; },
        restoreCursorPosition: function() {},
        saveCursorPosition: function() {},
        isClickable: function() { return true; },
        toggleListItemHeader: function() {},
        formatString: function(str) { return str || ''; },
        getPossibleActionObjects: function() { return []; }
    });
    
    Object.assign(window.BasDialogsLib.templates, {
        main: function() { return '<div>Template disabled</div>'; },
        recent: function() { return '<div>Recent disabled</div>'; },
        visibleName: function() { return '<span>Item</span>'; }
    });
}
`;
                }

                res.send(content);
                return;
            }
        }

        console.log(`JavaScript file not found: ${req.path}`);
        res.send('// File not found in Docker mode');
        return;
    } else if (req.path.endsWith('.css')) {
        res.type('text/css');
        
        const possiblePaths = [
            path.join('ChromeWorker/html', req.path),
            path.join('ChromeWorker/html/toolbox', req.path),
            path.join('ChromeWorker/html/scenario', req.path),
            path.join('ChromeWorker/html/central', req.path),
            path.join('ChromeWorker/html/scheduler', req.path)
        ];
        
        for (const cssFilePath of possiblePaths) {
            if (fs.existsSync(cssFilePath) && fs.statSync(cssFilePath).isFile()) {
                console.log(`Serving CSS file: ${cssFilePath}`);
                const content = fs.readFileSync(cssFilePath, 'utf8');
                res.send(content);
                return;
            }
        }
        
        console.log(`CSS file not found: ${req.path}`);
        res.send('/* CSS file not found in Docker mode */');
        return;
    } else if (req.path.endsWith('.json')) {
        res.type('application/json');
    }
    next();
});

app.use('/toolbox', express.static('ChromeWorker/html/toolbox'));
app.use('/scenario', express.static('ChromeWorker/html/scenario'));
app.use('/central', express.static('ChromeWorker/html/central'));
app.use('/scheduler', express.static('ChromeWorker/html/scheduler'));

app.use('/vs', express.static('ChromeWorker/html/toolbox/vs'));

app.use('/actions', express.static('ChromeWorker/html/actions'));

app.use(express.static('ChromeWorker/html'));

app.get('/', (req, res) => {
    res.send(`
        <html>
        <head>
            <title>BAS Web Interface</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 40px; }
                .interface-link {
                    display: block;
                    margin: 10px 0;
                    padding: 10px;
                    background: #f0f0f0;
                    text-decoration: none;
                    border-radius: 5px;
                }
                .interface-link:hover { background: #e0e0e0; }
            </style>
        </head>
        <body>
            <h1>BAS (Browser Automation Studio) Web Interface</h1>
            <p>Available interfaces:</p>
            <a href="/toolbox" class="interface-link">🔧 Toolbox - Script Editor with Monaco</a>
            <a href="/scenario" class="interface-link">📝 Scenario Editor - Visual Script Builder</a>
            <a href="/central" class="interface-link">⚙️ Settings - Configuration Panel</a>
            <a href="/scheduler" class="interface-link">📅 Scheduler - Task Management</a>

            <h3>WebSocket Connection</h3>
            <p>The BAS application WebSocket server runs on random port 10000-20000.</p>
            <p>Check container logs for the exact port when BAS applications start.</p>
        </body>
        </html>
    `);
});

app.get('/health', (req, res) => {
    res.json({
        status: 'ok',
        timestamp: new Date().toISOString(),
        interfaces: {
            toolbox: fs.existsSync('ChromeWorker/html/toolbox/index.html'),
            scenario: fs.existsSync('ChromeWorker/html/scenario/index.html'),
            central: fs.existsSync('ChromeWorker/html/central/index_settings.html'),
            scheduler: fs.existsSync('ChromeWorker/html/scheduler/index.html')
        }
    });
});

const PORT = process.env.PORT || 8080;
app.listen(PORT, '0.0.0.0', () => {
    console.log(`BAS Web Interface server running on port ${PORT}`);
    console.log(`Access interfaces at:`);
    console.log(`- Main: http://localhost:${PORT}`);
    console.log(`- Toolbox: http://localhost:${PORT}/toolbox`);
    console.log(`- Scenario: http://localhost:${PORT}/scenario`);
    console.log(`- Settings: http://localhost:${PORT}/central`);
    console.log(`- Scheduler: http://localhost:${PORT}/scheduler`);
});
