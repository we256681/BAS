const express = require('express');
const path = require('path');
const fs = require('fs');

const app = express();

/**
 * Robust Template Processor for Docker BAS Implementation
 * Handles complex template files with stub replacement capability
 */
class RobustTemplateProcessor {
    constructor() {
        this.stubFiles = new Set([
            'functionmanager.js',
            'tooledViewPlugin.js',
            'modal.js',
            'documents.js'
        ]);

        this.patterns = {
            allTags: /<%[-=]?(?:[^%]|%(?!>))*%>/g,
            // Execution blocks
            execution: /<%\s*(?!=)((?:[^%]|%(?!>))*)%>/g,
            output: /<%=\s*((?:[^%]|%(?!>))*)\s*%>/g,
            escapedOutput: /<%-\s*((?:[^%]|%(?!>))*)\s*%>/g,
            templateDensity: /<%(?:[^%]|%(?!>))*%>/g
        };

        this.stubs = {
            'functionmanager.js': this.createFunctionManagerStub(),
            'tooledViewPlugin.js': this.createTooledViewPluginStub(),
            'modal.js': this.createModalStub(),
            'documents.js': this.createDocumentsStub(),
            'default': this.createDefaultStub()
        };
    }

    /**
     * Main processing function with intelligent stub replacement
     * @param {string} content - File content
     * @param {string} filename - File name
     * @returns {object} - Processing result
     */
    processContent(content, filename = '') {
        try {
            const baseName = path.basename(filename);
            
            if (this.shouldStub(content, baseName)) {
                return {
                    success: true,
                    content: this.getStubContent(baseName),
                    method: 'stubbed',
                    reason: 'High template density or known problematic file'
                };
            }

            const methods = [
                () => this.aggressiveClean(content),
                () => this.safeTemplateRemoval(content),
                () => this.commentOutTemplates(content),
                () => this.getStubContent(baseName) // Fallback to stub
            ];

            for (let i = 0; i < methods.length; i++) {
                try {
                    const processed = methods[i]();
                    this.validateJavaScript(processed);
                    return {
                        success: true,
                        content: processed,
                        method: ['aggressive', 'safe', 'commented', 'stubbed'][i],
                        attempts: i + 1
                    };
                } catch (error) {
                    console.log(`Method ${i + 1} failed: ${error.message}`);
                    continue;
                }
            }

            return {
                success: true,
                content: this.getStubContent(baseName),
                method: 'stubbed',
                reason: 'All processing methods failed'
            };
        } catch (error) {
            return {
                success: false,
                error: error.message,
                content: this.getStubContent(filename)
            };
        }
    }

    /**
     * Check if file should be stubbed based on template density
     * @param {string} content - File content
     * @param {string} filename - File name
     * @returns {boolean}
     */
    shouldStub(content, filename) {
        if (this.stubFiles.has(filename)) {
            return true;
        }

        const templateMatches = content.match(this.patterns.templateDensity) || [];
        const templateChars = templateMatches.join('').length;
        const totalChars = content.length;
        const density = templateChars / totalChars;

        return density > 0.3;
    }

    /**
     * Aggressive template cleaning - removes all template syntax
     * @param {string} content - Content to clean
     * @returns {string} - Cleaned content
     */
    aggressiveClean(content) {
        let processed = content;

        // Remove all template tags completely
        processed = processed.replace(this.patterns.allTags, '');

        // Fix common syntax issues
        processed = this.fixSyntaxIssues(processed);

        // Clean up whitespace and empty lines
        processed = processed
            .replace(/\n\s*\n\s*\n/g, '\n\n')
            .replace(/^\s*\n/gm, '')
            .trim();

        return processed;
    }

    /**
     * Safe template removal - preserves structure
     * @param {string} content - Content to clean
     * @returns {string} - Cleaned content
     */
    safeTemplateRemoval(content) {
        let processed = content;

        processed = processed.replace(this.patterns.output, '"[OUTPUT]"');
        processed = processed.replace(this.patterns.escapedOutput, '"[ESCAPED_OUTPUT]"');

        processed = processed.replace(this.patterns.execution, '/* [TEMPLATE_CODE] */');

        // Fix syntax issues
        processed = this.fixSyntaxIssues(processed);

        return processed;
    }

    /**
     * Comment out all template code
     * @param {string} content - Content to clean
     * @returns {string} - Cleaned content
     */
    commentOutTemplates(content) {
        let processed = content;

        processed = processed.replace(this.patterns.allTags, '/* $& */');

        // Fix syntax issues
        processed = this.fixSyntaxIssues(processed);

        return processed;
    }

    /**
     * Fix common JavaScript syntax issues
     * @param {string} content - Content to fix
     * @returns {string} - Fixed content
     */
    fixSyntaxIssues(content) {
        return content
            // Fix orphaned else statements
            .replace(/\s*else\s*\{[^}]*\}/g, '')
            // Fix incomplete if statements
            .replace(/if\s*\(\s*\)\s*\{[^}]*\}/g, '')
            .replace(/\.\s*\(\s*\)/g, '.call()')
            .replace(/\+\s*\+/g, '+')
            .replace(/;;+/g, ';')
            .replace(/[+\-*/]\s*;/g, ';')
            // Remove empty statements
            .replace(/;\s*;/g, ';');
    }

    /**
     * Get stub content for a file
     * @param {string} filename - File name
     * @returns {string} - Stub content
     */
    getStubContent(filename) {
        const baseName = path.basename(filename);
        return this.stubs[baseName] || this.stubs.default;
    }

    /**
     * Validate JavaScript syntax
     * @param {string} code - JavaScript code
     * @throws {Error} - If invalid
     */
    validateJavaScript(code) {
        try {
            new Function(code);
        } catch (error) {
            throw new Error(`Invalid JavaScript: ${error.message}`);
        }
    }

    createFunctionManagerStub() {
        return `
var FunctionManager = {
    init: function() {
        console.log('FunctionManager initialized (stub)');
    },
    execute: function(funcName, params) {
        console.log('FunctionManager.execute called:', funcName);
        return null;
    },
    register: function(name, func) {
        console.log('FunctionManager.register called:', name);
    },
    get: function(name) {
        console.log('FunctionManager.get called:', name);
        return function() { return null; };
    }
};

if (typeof module !== 'undefined' && module.exports) {
    module.exports = FunctionManager;
}
`.trim();
    }

    createTooledViewPluginStub() {
        return `
var TooledViewPlugin = {
    init: function() {
        console.log('TooledViewPlugin initialized (stub)');
    },
    render: function(container, data) {
        console.log('TooledViewPlugin.render called');
        if (container) {
            container.innerHTML = '<div>TooledViewPlugin Stub</div>';
        }
    },
    update: function(data) {
        console.log('TooledViewPlugin.update called');
    }
};

if (typeof module !== 'undefined' && module.exports) {
    module.exports = TooledViewPlugin;
}
`.trim();
    }

    createModalStub() {
        return `
var Modal = {
    show: function(options) {
        console.log('Modal.show called (stub)');
        return {
            hide: function() {
                console.log('Modal.hide called (stub)');
            }
        };
    },
    hide: function() {
        console.log('Modal.hide called (stub)');
    },
    confirm: function(message, callback) {
        console.log('Modal.confirm called:', message);
        if (callback) callback(true);
    }
};

if (typeof module !== 'undefined' && module.exports) {
    module.exports = Modal;
}
`.trim();
    }

    createDocumentsStub() {
        return `
var Documents = {
    init: function() {
        console.log('Documents initialized (stub)');
    },
    load: function(id, callback) {
        console.log('Documents.load called:', id);
        if (callback) callback(null, { id: id, content: 'stub content' });
    },
    save: function(doc, callback) {
        console.log('Documents.save called:', doc.id);
        if (callback) callback(null, doc);
    },
    list: function(callback) {
        console.log('Documents.list called');
        if (callback) callback(null, []);
    }
};

if (typeof module !== 'undefined' && module.exports) {
    module.exports = Documents;
}
`.trim();
    }

    createDefaultStub() {
        return `
(function() {
    'use strict';
    console.log('Template file loaded as stub');
    
    var stub = {
        init: function() {
            console.log('Stub initialized');
        }
    };
    
    if (typeof module !== 'undefined' && module.exports) {
        module.exports = stub;
    }
    
    if (typeof define === 'function' && define.amd) {
        define([], function() {
            return stub;
        });
    }
    
    if (typeof window !== 'undefined') {
        window.TemplateStub = stub;
    }
})();
`.trim();
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
 * Detect if file is mostly template content (ChatGPT recommendation)
 * Files with >50% template tags should be handled differently
 */
function isMostlyTemplate(content) {
    const templateTags = content.match(/<%[-=]?[\s\S]*?%>/g) || [];
    const ratio = templateTags.join('').length / content.length;
    return ratio > 0.5; // >50% considered template-heavy
}

/**
 * Comprehensive Underscore.js Template Cleaner from Claude AI
 * Processes and cleans Underscore.js templates to eliminate JavaScript syntax errors
 * while maintaining valid JavaScript code structure.
 */
class UnderscoreTemplateCleaner {
    constructor(options = {}) {
        this.options = {
            preserveComments: options.preserveComments || false,
            addDebugInfo: options.addDebugInfo || false,
            replaceWithPlaceholders: options.replaceWithPlaceholders || true,
            ...options
        };

        this.patterns = {
            // Execution templates: <% code %>
            execution: /<%\s*(?![-=])([\s\S]*?)%>/g,
            output: /<%=\s*([\s\S]*?)%>/g,
            escaped: /<%-\s*([\s\S]*?)%>/g,
            // Combined pattern for all templates
            all: /<%[-=]?\s*([\s\S]*?)%>/g
        };
    }

    /**
     * Main cleaning function - processes Underscore.js templates
     * @param {string} content - The template content to clean
     * @returns {string} - Cleaned JavaScript content
     */
    clean(content) {
        if (!content || typeof content !== 'string') {
            return '';
        }

        try {
            let cleaned = content;

            cleaned = this.processExecutionTemplates(cleaned);

            cleaned = this.processOutputTemplates(cleaned);

            cleaned = this.processEscapedTemplates(cleaned);

            // Step 4: Clean up remaining template artifacts
            cleaned = this.cleanTemplateArtifacts(cleaned);

            // Step 5: Fix JavaScript syntax issues
            cleaned = this.fixJavaScriptSyntax(cleaned);

            // Step 6: Validate and repair structure
            cleaned = this.validateAndRepair(cleaned);

            return cleaned;
        } catch (error) {
            console.error('Template cleaning error:', error);
            return this.createFallbackContent(content);
        }
    }

    /**
     * Process execution templates like <% if (condition) { %>
     */
    processExecutionTemplates(content) {
        return content.replace(this.patterns.execution, (match, code) => {
            const cleanCode = this.cleanCodeBlock(code);

            if (this.options.replaceWithPlaceholders) {
                return `/* TEMPLATE_EXEC: ${cleanCode.replace(/\*\//g, '*_/')} */`;
            }

            return this.makeValidJavaScript(cleanCode);
        });
    }

    /**
     * Process output templates like <%= variable %>
     */
    processOutputTemplates(content) {
        return content.replace(this.patterns.output, (match, expression) => {
            const cleanExpr = this.cleanExpression(expression);

            if (this.options.replaceWithPlaceholders) {
                return `/* TEMPLATE_OUTPUT: ${cleanExpr.replace(/\*\//g, '*_/')} */`;
            }

            return `(${cleanExpr})`;
        });
    }

    /**
     * Process escaped output templates like <%- variable %>
     */
    processEscapedTemplates(content) {
        return content.replace(this.patterns.escaped, (match, expression) => {
            const cleanExpr = this.cleanExpression(expression);

            if (this.options.replaceWithPlaceholders) {
                return `/* TEMPLATE_ESCAPED: ${cleanExpr.replace(/\*\//g, '*_/')} */`;
            }

            return `(${cleanExpr} /* escaped */)`;
        });
    }

    /**
     * Clean individual code blocks
     */
    cleanCodeBlock(code) {
        return code
            .trim()
            .replace(/^\s*\n|\n\s*$/g, '') // Remove leading/trailing newlines
            .replace(/\s+/g, ' ') // Normalize whitespace
            .replace(/['"]/g, match => match === '"' ? '\\"' : "\\'"); // Escape quotes
    }

    /**
     * Clean individual expressions
     */
    cleanExpression(expression) {
        return expression
            .trim()
            .replace(/^\s*\n|\n\s*$/g, '')
            .replace(/\s+/g, ' ')
            .replace(/['"]/g, match => match === '"' ? '\\"' : "\\'");
    }

    /**
     * Clean remaining template artifacts
     */
    cleanTemplateArtifacts(content) {
        return content
            // Remove any remaining template tags
            .replace(/<%[\s\S]*?%>/g, '/* TEMPLATE_REMOVED */')
            // Clean up multiple consecutive newlines
            .replace(/\n\s*\n\s*\n/g, '\n\n')
            // Remove trailing whitespace
            .replace(/[ \t]+$/gm, '');
    }

    /**
     * Fix common JavaScript syntax issues
     */
    fixJavaScriptSyntax(content) {
        return content
            // Fix dangling else statements
            .replace(/^\s*else\s*{/gm, '/* else */ {')
            // Fix incomplete if statements
            .replace(/if\s*\(\s*\)\s*{/g, 'if (true) {')
            // Fix incomplete for loops
            .replace(/for\s*\(\s*\)\s*{/g, 'for (;;) {')
            // Fix incomplete while loops
            .replace(/while\s*\(\s*\)\s*{/g, 'while (true) {')
            .replace(/\.\s*$/gm, '/* dot */')
            // Fix incomplete object access
            .replace(/\.\s*\n/g, '/* dot */\n')
            // Fix incomplete method calls
            .replace(/\(\s*$/gm, '(/* params */')
            .replace(/{\s*$/gm, '{ /* block */ }')
            .replace(/,(\s*[}\]])/g, '$1');
    }

    /**
     * Validate and repair JavaScript structure
     */
    validateAndRepair(content) {
        const openBraces = (content.match(/{/g) || []).length;
        const closeBraces = (content.match(/}/g) || []).length;
        const openBrackets = (content.match(/\[/g) || []).length;
        const closeBrackets = (content.match(/\]/g) || []).length;
        const openParens = (content.match(/\(/g) || []).length;
        const closeParens = (content.match(/\)/g) || []).length;

        let repaired = content;

        if (openBraces > closeBraces) {
            repaired += '\n' + '}'.repeat(openBraces - closeBraces);
        }

        if (openBrackets > closeBrackets) {
            repaired += ']'.repeat(openBrackets - closeBrackets);
        }

        if (openParens > closeParens) {
            repaired += ')'.repeat(openParens - closeParens);
        }

        return repaired;
    }

    /**
     * Convert code to valid JavaScript when possible
     */
    makeValidJavaScript(code) {
        if (code.match(/^\s*if\s*\(/)) {
            return `if (${code.replace(/^\s*if\s*\(/, '').replace(/\)\s*{\s*$/, '')}){`;
        }

        if (code.match(/^\s*else\s*{/)) {
            return '} else {';
        }

        if (code.match(/^\s*}\s*else\s*{/)) {
            return '} else {';
        }

        if (code.match(/^\s*}/)) {
            return '}';
        }

        if (code.match(/^\s*for\s*\(/)) {
            return `for(${code.replace(/^\s*for\s*\(/, '').replace(/\)\s*{\s*$/, '')}){`;
        }

        if (code.match(/^\s*while\s*\(/)) {
            return `while(${code.replace(/^\s*while\s*\(/, '').replace(/\)\s*{\s*$/, '')}){`;
        }

        return `/* ${code} */`;
    }

    /**
     * Create fallback content when cleaning fails
     */
    createFallbackContent(originalContent) {
        return `/* TEMPLATE_CONTENT_CLEANED */\n${originalContent.replace(/<%[\s\S]*?%>/g, '/* TEMPLATE */')}\n/* END_TEMPLATE_CONTENT */`;
    }
}

/**
 * Quick clean function for simple use cases - Claude AI's solution
 */
function cleanUnderscoreTemplates(content, options = {}) {
    const cleaner = new UnderscoreTemplateCleaner(options);
    return cleaner.clean(content);
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

                const templateProcessor = new RobustTemplateProcessor();
                const shouldStub = templateProcessor.shouldStub(content, path.basename(jsFilePath));

                if (shouldStub) {
                    console.log(`Creating comprehensive stub for template-heavy file: ${jsFilePath} (${templateProcessor.stubFiles.has(path.basename(jsFilePath)) ? 'predefined' : 'auto-detected'})`);

                    if (!templateProcessor.stubFiles.has(path.basename(jsFilePath))) {
                        console.log(`File is ${Math.round((content.match(/<%[-=]?[\s\S]*?%>/g) || []).join('').length / content.length * 100)}% template content - using complete replacement`);
                        
                        const fileName = path.basename(jsFilePath);
                        content = `
// DOCKER MODE: Template-heavy file replaced with functional stub
// Original file: ${fileName} (${Math.round((content.match(/<%[-=]?[\s\S]*?%>/g) || []).join('').length / content.length * 100)}% template content)
console.log('Docker stub loaded: ${fileName}');

if (typeof window !== 'undefined') {
    window.BasDialogsLib = window.BasDialogsLib || {};
    window.BasDialogsLib.utils = window.BasDialogsLib.utils || {};
    window.BasDialogsLib.templates = window.BasDialogsLib.templates || {};
}
`;
                    } else {
                        const templateProcessor = new RobustTemplateProcessor();
                        
                        try {
                            content = templateProcessor.processContent(content, path.basename(jsFilePath));
                        } catch (error) {
                            console.log(`Template processing failed, using fallback: ${error.message}`);
                            content = templateProcessor.aggressiveClean(content);
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

                if (jsFilePath.includes('functionmanager.js') || 
                    jsFilePath.includes('tooledViewPlugin.js') ||
                    jsFilePath.includes('documents.js') ||
                    jsFilePath.includes('modal.js')) {
                    
                    console.log(`Creating comprehensive stub for template-heavy file: ${jsFilePath}`);
                    
                    if (content.includes('var Template = `') || 
                        content.includes('let Template = `') || 
                        content.includes('const Template = `')) {
                        content = cleanTemplateStrings(content);
                    } else {
                        content = cleanUnderscoreTemplates(content);
                    }
                    
                    if (jsFilePath.includes('functionmanager.js')) {
                        content += `
function FunctionManager() {
    this.RenderDefault = function() {};
    this.SendCurrentFunction = function() {};
    this.ChangeFunction = function() {};
    this.Render = function() {};
    this.Show = function() {};
    this.Hide = function() {};
    this.IsVisible = function() { return false; };
}`;
                    }
                } else if (content.includes('<%') && content.includes('%>')) {
                    console.log(`Applying template processing to file with template syntax: ${jsFilePath}`);
                    content = cleanUnderscoreTemplates(content);
                } else {
                    console.log(`Serving JavaScript file as-is (no template syntax): ${jsFilePath}`);
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
