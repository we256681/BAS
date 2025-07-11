    ;_BAS_HIDE(DomPredictionHelper) = function(_bas_use_css, _bas_use_ids, _bas_check_density, _query_selector_host) {
      this.recursiveNodes = function(e) {
        var n;
        if (_BAS_SAFE($Node.nodeName)(e) && _BAS_SAFE($Node.parentNode)(e) && e !== _BAS_SAFE($Document.body)(document) && !(e instanceof _BAS_SAFE(ShadowRoot)) && !(_BAS_SAFE($Node.parentNode)(e) instanceof _BAS_SAFE(ShadowRoot))) {
          n = this.recursiveNodes(_BAS_SAFE($Node.parentNode)(e));
        } else {
          n = [];
        }
        _BAS_SAFE($Array.push)(n, e);
        return n;
      };
  
      this.escapeCssNames = function(name) {
        if (name) {
          try {
            let replacements = [
              [/\bselectorgadget_\w+\b/g, ''],
              [/\\/g, '\\\\'],
              [/[\#\;\&\,\.\+\*\~\'\:\"\!\^\$\[\]\(\)\=\>\|\/]/g, (e) => '\\' + e],
              [/\s+/, '']
            ]
            for (let replacement of replacements) name = _BAS_SAFE($String.replace)(name, ...replacement);
            return name;
          } catch (e) {
            return '';
          }
        } else {
          return '';
        }
      };
  
      this.childElemNumber = function(elem) {
        var count;
        count = 0;
        while (_BAS_SAFE($Node.previousSibling)(elem) && (elem = _BAS_SAFE($Node.previousSibling)(elem))) {
          if (_BAS_SAFE($Node.nodeType)(elem) === 1) {
            count++;
          }
        }
        return count;
      };
  
      this.siblingsWithoutTextNodes = function(e) {
        var filtered_nodes, node, nodes, _i, _len;
        if(_BAS_SAFE($Node.parentNode)(e))
          nodes = _BAS_HIDE(BrowserAutomationStudio_ConvertIfNeeded)(
            _BAS_SAFE($Node.childNodes)(_BAS_SAFE($Node.parentNode)(e))
          );
        else
          nodes = []
        filtered_nodes = [];
        for (_i = 0, _len = nodes.length; _i < _len; _i++) {
          node = nodes[_i];
          if (_BAS_SAFE($String.substring)(_BAS_SAFE($Node.nodeName)(node), 0, 1) === "#") {
            continue;
          }
          if (node === e) {
            break;
          }
          _BAS_SAFE($Array.push)(filtered_nodes, node);
        }
        return filtered_nodes;
      };
  
      this.pathOf = function(elem) {
        var e, j, path, siblings, _i, _len, _ref;
        path = "";
        _ref = this.recursiveNodes(elem);
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          e = _ref[_i];
          if (e) {
            siblings = this.siblingsWithoutTextNodes(e);
            if (_BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.nodeName)(e)) !== "body") {
              j = siblings.length - 2 < 0 ? 0 : siblings.length - 2;
              while (j < siblings.length) {
                if (siblings[j] === e) {
                  break;
                }
                if (!_BAS_SAFE($String.match)(_BAS_SAFE($Node.nodeName)(siblings[j]), /^(script|#.*?)$/i)) {
                  path += this.cssDescriptor(siblings[j]) + (j + 1 === siblings.length ? "+ " : "~ ");
                }
                j++;
              }
            }
            path += this.cssDescriptor(e) + " > ";
          }
        }
        return this.cleanCss(path);
      };
  
      this.cssDescriptor = function(node) {
        var cssName, escaped, path, _i, _len, _ref;
        path = _BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.nodeName)(node));
        let nodeId = _BAS_SAFE($Element.id)(node);
        escaped = nodeId && this.escapeCssNames(nodeId);
        if(_bas_use_ids)
        {
            if (escaped && escaped.length > 0)
            {
                path += '#' + escaped;
            }
        }
        if(_bas_use_css)
        {
            let className = _BAS_SAFE($Element.className)(node);
            if (className) {
            _ref = _BAS_SAFE($String.split)(className, " ");
                for (_i = 0, _len = _ref.length; _i < _len; _i++) {
                    cssName = _ref[_i];
                    escaped = this.escapeCssNames(cssName);
                    if (cssName && escaped.length > 0) {
                    path += '.' + escaped;
                    }
                }
            }
        }
        if (_BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.nodeName)(node)) !== "body") {
          path += ':nth-child(' + (this.childElemNumber(node) + 1) + ')';
        }
        return path;
      };
  
      this.cssDiff = function(array) {
        var collective_common, cssElem, diff, dmp, encoded_css_array, existing_tokens, part, _i, _j, _len, _len1;
        try {
          dmp = new _BAS_HIDE(diff_match_patch)();
        } catch (e) {
          throw "Please include the diff_match_patch library.";
        }
        if (typeof array === 'undefined' || array.length === 0) {
          return '';
        }
        existing_tokens = {};
        encoded_css_array = this.encodeCssForDiff(array, existing_tokens);
        collective_common = _BAS_SAFE($Array.pop)(encoded_css_array);
        for (_i = 0, _len = encoded_css_array.length; _i < _len; _i++) {
          cssElem = encoded_css_array[_i];
          diff = dmp.diff_main(collective_common, cssElem);
          collective_common = '';
          for (_j = 0, _len1 = diff.length; _j < _len1; _j++) {
            part = diff[_j];
            if (part[0] === 0) {
              collective_common += part[1];
            }
          }
        }
        return this.decodeCss(collective_common, existing_tokens);
      };
  
      this.tokenizeCss = function(css_string) {
        var char, skip, tokens, word, _i, _len, _ref;
        skip = false;
        word = '';
        tokens = [];
        _ref = this.cleanCss(css_string);
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          char = _ref[_i];
          if (skip) {
            skip = false;
          } else if (char === '\\') {
            skip = true;
          } else if (char === '.' || char === ' ' || char === '#' || char === '>' || char === ':' || char === ',' || char === '+' || char === '~') {
            if (word.length > 0) {
              _BAS_SAFE($Array.push)(tokens, word);
            }
            word = '';
          }
          word += char;
          if (char === ' ' || char === ',') {
            _BAS_SAFE($Array.push)(tokens, word);
            word = '';
          }
        }
        if (word.length > 0) {
          _BAS_SAFE($Array.push)(tokens, word);
        }
        return tokens;
      };
  
      this.tokenizeCssForDiff = function(css_string) {
        var block, combined_tokens, token, _i, _len, _ref;
        combined_tokens = [];
        block = [];
        _ref = this.tokenizeCss(css_string);
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          token = _ref[_i];
          _BAS_SAFE($Array.push)(block, token);
          if (token === ' ' && block.length > 0) {
            combined_tokens = _BAS_SAFE($Array.concat)(combined_tokens, block);
            block = [];
          } else if (token === '+' || token === '~') {
            block = [_BAS_SAFE($Array.join)(block, '')];
          }
        }
        if (block.length > 0) {
          return _BAS_SAFE($Array.concat)(combined_tokens, block);
        } else {
          return combined_tokens;
        }
      };
  
      this.decodeCss = function(string, existing_tokens) {
        var character, inverted, out, _i, _len, _ref;
        inverted = this.invertObject(existing_tokens);
        out = '';
        _ref = _BAS_SAFE($String.split)(string, '');
        for (_i = 0, _len = _ref.length; _i < _len; _i++) {
          character = _ref[_i];
          out += inverted[character];
        }
        return this.cleanCss(out);
      };
  
      this.encodeCssForDiff = function(strings, existing_tokens) {
        var codepoint, out, string, strings_out, token, _i, _j, _len, _len1, _ref;
        codepoint = 50;
        strings_out = [];
        for (_i = 0, _len = strings.length; _i < _len; _i++) {
          string = strings[_i];
          out = '';
          _ref = this.tokenizeCssForDiff(string);
          for (_j = 0, _len1 = _ref.length; _j < _len1; _j++) {
            token = _ref[_j];
            if (!existing_tokens[token]) {
              existing_tokens[token] = _BAS_SAFE(String.fromCharCode)(codepoint++);
            }
            out += existing_tokens[token];
          }
          _BAS_SAFE($Array.push)(strings_out, out);
        }
        return strings_out;
      };
  
      this.tokenPriorities = function(tokens) {
        var epsilon, first, i, priorities, second, token, _i, _len;
        epsilon = 0.001;
        priorities = [];
        i = 0;
        for (_i = 0, _len = tokens.length; _i < _len; _i++) {
          token = tokens[_i];
          first = _BAS_SAFE($String.substring)(token, 0, 1);
          second = _BAS_SAFE($String.substring)(token, 1, 2);
          if (first === ':' && second === 'n') {
            priorities[i] = 0;
          } else if (first === '>') {
            priorities[i] = 2;
          } else if (first === '+' || first === '~') {
            priorities[i] = 3;
          } else if (first !== ':' && first !== '.' && first !== '#' && first !== ' ' && first !== '>' && first !== '+' && first !== '~') {
            priorities[i] = 4;
          } else if (first === '.') {
            priorities[i] = 5;
          } else if (first = '#') {
            priorities[i] = 6;
            if (_BAS_SAFE($String.match)(token, /\d{3,}/)) {
              priorities[i] = 2.5;
            }
          } else {
            priorities[i] = 0;
          }
          priorities[i] += i * epsilon;
          i++;
        }
        return priorities;
      };
  
      this.orderFromPriorities = function(priorities) {
        var i, ordering, tmp, _i, _j, _ref, _ref1;
        tmp = [];
        ordering = [];
        for (i = _i = 0, _ref = priorities.length; 0 <= _ref ? _i < _ref : _i > _ref; i = 0 <= _ref ? ++_i : --_i) {
          tmp[i] = {
            value: priorities[i],
            original: i
          };
        }
        _BAS_SAFE($Array.sort)(tmp, function(a, b) {
          return a.value - b.value;
        });
        for (i = _j = 0, _ref1 = priorities.length; 0 <= _ref1 ? _j < _ref1 : _j > _ref1; i = 0 <= _ref1 ? ++_j : --_j) {
          ordering[i] = tmp[i].original;
        }
        return ordering;
      };
  
      this.simplifyCss = function(css, selected, rejected) {
        var self = this
        var best_so_far, first, got_shorter, i, look_back_index, ordering, part, parts, priorities, second, selector, _i, _ref,
          _this = this;
        parts = this.tokenizeCss(css);
        priorities = this.tokenPriorities(parts);
        ordering = this.orderFromPriorities(priorities);
        selector = this.cleanCss(css);
        look_back_index = -1;
        best_so_far = "";
        if (this.selectorGets('all', selected, selector) && this.selectorGets('none', rejected, selector)) {
          best_so_far = selector;
        }
        got_shorter = true;
        while (got_shorter) {
          got_shorter = false;
          for (i = _i = 0, _ref = parts.length; 0 <= _ref ? _i < _ref : _i > _ref; i = 0 <= _ref ? ++_i : --_i) {
            part = ordering[i];
            if (parts[part].length === 0) {
              continue;
            }
            first = _BAS_SAFE($String.substring)(parts[part], 0, 1);
            second = _BAS_SAFE($String.substring)(parts[part], 1, 2);
            if (first === ' ') {
              continue;
            }
            if (this.wouldLeaveFreeFloatingNthChild(parts, part)) {
              continue;
            }
            this._removeElements(part, parts, first, function(selector) {
              if (_this.selectorGets('all', selected, selector) && _this.selectorGets('none', rejected, selector) && (selector.length < best_so_far.length || best_so_far.length === 0)) {
                if(_bas_check_density && self.getQueryDensity(selector) > 5)
                {
                  return false
                }
                best_so_far = selector;
                got_shorter = true;
                return true;
              } else {
                return false;
              }
            });
          }
        }
        return this.cleanCss(best_so_far);
      };
  
      this._removeElements = function(part, parts, firstChar, callback) {
        var j, look_back_index, selector, tmp, _i, _j;
        if (firstChar === '+' || firstChar === '~') {
          look_back_index = this.positionOfSpaceBeforeIndexOrLineStart(part, parts);
        } else {
          look_back_index = part;
        }
        tmp = _BAS_SAFE($Array.slice)(parts, look_back_index, part + 1);
        for (j = _i = look_back_index; look_back_index <= part ? _i <= part : _i >= part; j = look_back_index <= part ? ++_i : --_i) {
          parts[j] = '';
        }
        selector = this.cleanCss(_BAS_SAFE($Array.join)(parts, ''));
        if (selector === '' || !callback(selector)) {
          for (j = _j = look_back_index; look_back_index <= part ? _j <= part : _j >= part; j = look_back_index <= part ? ++_j : --_j) {
            parts[j] = tmp[j - look_back_index];
          }
        }
        return parts;
      };
  
      this.positionOfSpaceBeforeIndexOrLineStart = function(part, parts) {
        var i;
        i = part;
        while (i >= 0 && parts[i] !== ' ') {
          i--;
        }
        if (i < 0) {
          i = 0;
        }
        return i;
      };
  
      this.wouldLeaveFreeFloatingNthChild = function(parts, part) {
        var i, nth_child_is_on_right, space_is_on_left;
        space_is_on_left = nth_child_is_on_right = false;
        i = part + 1;
        while (i < parts.length && parts[i].length === 0) {
          i++;
        }
        if (i < parts.length && _BAS_SAFE($String.substring)(parts[i], 0, 2) === ':n') {
          nth_child_is_on_right = true;
        }
        i = part - 1;
        while (i > -1 && parts[i].length === 0) {
          i--;
        }
        if (i < 0 || parts[i] === ' ') {
          space_is_on_left = true;
        }
        return space_is_on_left && nth_child_is_on_right;
      };
  
      this.cleanCss = function(css) {
        var cleaned_css, last_cleaned_css;
        cleaned_css = css;
        last_cleaned_css = null;
        let replacements = [
          [/(^|\s+)(\+|\~)/, ''],
          [/(\+|\~)\s*$/, ''],
          [/>/g, ' > '],
          [/\s*(>\s*)+/g, ' > '],
          [/,/g, ' , '],
          [/\s+/g, ' '],
          [/^\s+|\s+$/g, ''],
          [/\s*,$/g, ''],
          [/^\s*,\s*/g, ''],
          [/\s*>$/g, ''],
          [/^>\s*/g, ''],
          [/[\+\~\>]\s*,/g, ','],
          [/[\+\~]\s*>/g, '>'],
          [/\s*(,\s*)+/g, ' , '],
        ]
        while (last_cleaned_css !== cleaned_css) {
          last_cleaned_css = cleaned_css;
          for (let replacement of replacements) cleaned_css = _BAS_SAFE($String.replace)(cleaned_css, ...replacement);
        }
        return cleaned_css;
      };
  
      this.getPathsFor = function(nodeset) {
        var node, out, _i, _len;
        out = [];
        for (_i = 0, _len = nodeset.length; _i < _len; _i++) {
          node = nodeset[_i];
          if (node && _BAS_SAFE($Node.nodeName)(node)) {
            _BAS_SAFE($Array.push)(out, this.pathOf(node));
          }
        }
        return out;
      };
  
      this.predictCss = function(s, r) {
        var css, selected, selected_paths, simplest, union, _i, _len;
        if (s.length === 0) {
          return '';
        }
        selected_paths = this.getPathsFor(s);
        css = this.cssDiff(selected_paths);
        simplest = this.simplifyCss(css, s, r);
        if (simplest.length > 0) {
          return simplest;
        }
        union = '';
        for (_i = 0, _len = s.length; _i < _len; _i++) {
          selected = s[_i];
          union = this.pathOf(selected) + ", " + union;
        }
        union = this.cleanCss(union);
        return this.simplifyCss(union, s, r);
      };
  
      this.selectorGets = function(type, list, the_selector) {
        if (list.length === 0 && type === 'all') {
          return false;
        }
        if (list.length === 0 && type === 'none') {
          return true;
        }
        try {
          if (type === 'all') {
            //return list.not(the_selector).length === 0;
            var all_matches = true;
            for(var i = 0;i<list.length;i++)
            {
                var el = list[i]
                var matches = false;
                try
                {
                  matches = _BAS_SAFE($Element.matches)(el, the_selector)
                }catch(e)
                {
                  matches = false
                }
                if(!matches)
                {
                    all_matches = false;
                    break;
                }
            }
            return all_matches;
          } else {
            //return !(list.is(the_selector));
            var at_least_one_match = false;
            for(var i = 0;i<list.length;i++)
            {
                var el = list[i]
                var matches = false;
                try
                {
                  matches = _BAS_SAFE($Element.matches)(el, the_selector)
                }catch(e)
                {
                  matches = false
                }
                if(matches)
                {
                    at_least_one_match = true;
                    break;
                }
            }
            return !at_least_one_match;
          }
        } catch (e) {
          throw e;
        }
      };
  
      this.invertObject = function(object) {
        var key, new_object, value;
        new_object = {};
        for (key in object) {
          value = object[key];
          new_object[value] = key;
        }
        return new_object;
      };
  
      this.cssToXPath = function(css_string) {
        var css_block, out, token, tokens, _i, _len;
        tokens = this.tokenizeCss(css_string);
        if (tokens[0] && tokens[0] === ' ') {
          _BAS_SAFE($Array.splice)(tokens, 0, 1);
        }
        if (tokens[tokens.length - 1] && tokens[tokens.length - 1] === ' ') {
          _BAS_SAFE($Array.splice)(tokens, tokens.length - 1, 1);
        }
        css_block = [];
        out = "";
        for (_i = 0, _len = tokens.length; _i < _len; _i++) {
          token = tokens[_i];
          if (token === ' ') {
            out += this.cssToXPathBlockHelper(css_block);
            css_block = [];
          } else {
            _BAS_SAFE($Array.push)(css_block, token);
          }
        }
        return out + this.cssToXPathBlockHelper(css_block);
      };
  
      this.cssToXPathBlockHelper = function(css_block) {
        var current, expressions, first, i, out, re, rest, _i, _j, _len, _ref;
        if (css_block.length === 0) {
          return '//';
        }
        out = '//';
        first = _BAS_SAFE($String.substring)(css_block[0], 0, 1);
        if (first === ',') {
          return " | ";
        }
        if (first === ':' || first === '#' || first === '.') {
          out += '*';
        }
        expressions = [];
        re = null;
        for (_i = 0, _len = css_block.length; _i < _len; _i++) {
          current = css_block[_i];
          first = _BAS_SAFE($String.substring)(current, 0, 1);
          rest = _BAS_SAFE($String.substring)(current, 1);
          if (first === ':') {
            if (re = _BAS_SAFE($String.match)(rest, /^nth-child\((\d+)\)$/)) {
              _BAS_SAFE($Array.push)(expressions, '(((count(preceding-sibling::*) + 1) = ' + re[1] + ') and parent::*)');
            }
          } else if (first === '.') {
            _BAS_SAFE($Array.push)(expressions, 'contains(concat( " ", @class, " " ), concat( " ", "' + rest + '", " " ))');
          } else if (first === '#') {
            _BAS_SAFE($Array.push)(expressions, '(@id = "' + rest + '")');
          } else if (first === ',') {
  
          } else {
            out += current;
          }
        }
        if (expressions.length > 0) {
          out += '[';
        }
        for (i = _j = 0, _ref = expressions.length; 0 <= _ref ? _j < _ref : _j > _ref; i = 0 <= _ref ? ++_j : --_j) {
          out += expressions[i];
          if (i < expressions.length - 1) {
            out += ' and ';
          }
        }
        if (expressions.length > 0) {
          out += ']';
        }
        return out;
      };

      this.getAllRectangles = function(elements)
      {
        var len = elements.length
        var res = []
        for(var i = 0;i<len;i++)
        {
          var element = elements[i]
          if(element && _BAS_HIDE(BrowserAutomationStudio_IsVisible)(element, false))
          {
            var rect = _BAS_HIDE(BrowserAutomationStudio_GetInternalBoundingRect)(element);
            if(rect.width > 0 && rect.height > 0)
              _BAS_SAFE($Array.push)(res, rect)
          }
        }
        return res;
      }

      this.getRandomBetween = function(min, max)
      {
        return _BAS_SAFE(Math.floor)(_BAS_SAFE(Math.random)() * (max + 1 - min) + min);
      }

      this.getRandomPoint = function(rectangle)
      {
        return {x:this.getRandomBetween(rectangle.left,rectangle.right),y:this.getRandomBetween(rectangle.top,rectangle.bottom)}
      }

      this.isPointInsideRectangle = function(point, rectangle)
      {
        return point.x >= rectangle.left && point.x <= rectangle.right
               && point.y >= rectangle.top && point.y <= rectangle.bottom
      }

      this.CacheElements = []
      this.CacheDensity = 0

      this.getQueryDensity = function(query)
      {
        var total = 0;
        var precision = 1;

        var elements = []
        
        try
        {
          elements = _BAS_HIDE(BrowserAutomationStudio_ConvertIfNeeded)(
            _BAS_SAFE($Node.querySelectorAll)(_query_selector_host, query)
          );
        }catch(e)
        {
          elements = []
        }

        if(elements.length == this.CacheElements.length)
        {
          var IsSame = true
          for(var i = 0;i<elements.length;i++)
          {
            if(elements[i] != this.CacheElements[i])
            {
              IsSame = false
              break
            }
          }
          if(IsSame)
          {
            return this.CacheDensity
          }
        }
        var rectangles = this.getAllRectangles(elements)
        var len = rectangles.length

        for(var i = 0;i<len;i++)
        {
          var rectangle_outter = rectangles[i]
          for(var p = 0;p<precision;p++)
          {
            var point = this.getRandomPoint(rectangle_outter)
            for(var j = 0;j<len;j++)
            {
              var rectangle_inner = rectangles[j]
              if(this.isPointInsideRectangle(point,rectangle_inner))
                total ++
            }
          }
        }
        
        if(len == 0)
          return 0.0

        var res = total / (len * precision)
        this.CacheElements = elements
        this.CacheDensity = res
        return res
      }


      this.css2xpath = (function () {
        var xpath_to_lower         = function (s) {
              return 'translate(' +
                      (s || 'normalize-space()') +
                      ', \'ABCDEFGHJIKLMNOPQRSTUVWXYZ\'' +
                      ', \'abcdefghjiklmnopqrstuvwxyz\')';
            },
            xpath_ends_with        = function (s1, s2) {
              return 'substring(' + s1 + ',' +
                      'string-length(' + s1 + ')-string-length(' + s2 + ')+1)=' + s2;
            },
            xpath_url              = function (s) {
              return 'substring-before(concat(substring-after(' +
                      (s || xpath_url_attrs) + ',"://"),"?"),"?")';
            },
            xpath_url_path         = function (s) {
              return 'substring-after(' + (s || xpath_url_attrs) + ',"/")';
            },
            xpath_url_domain       = function (s) {
              return 'substring-before(concat(substring-after(' +
                     (s || xpath_url_attrs) + ',"://"),"/"),"/")';
            },
            xpath_url_attrs        = '@href|@src',
            xpath_lower_case       = xpath_to_lower(),
            xpath_ns_uri           = 'ancestor-or-self::*[last()]/@url',
            xpath_ns_path          = xpath_url_path(xpath_url(xpath_ns_uri)),
            xpath_has_protocal     = '(starts-with(' + xpath_url_attrs + ',"http://") or starts-with(' + xpath_url_attrs + ',"https://"))',
            xpath_is_internal      = 'starts-with(' + xpath_url() + ',' + xpath_url_domain(xpath_ns_uri) + ') or ' + xpath_ends_with(xpath_url_domain(), xpath_url_domain(xpath_ns_uri)),
            xpath_is_local         = '(' + xpath_has_protocal + ' and starts-with(' + xpath_url() + ',' + xpath_url(xpath_ns_uri) + '))',
            xpath_is_path          = 'starts-with(' + xpath_url_attrs + ',"/")',
            xpath_is_local_path    = 'starts-with(' + xpath_url_path() + ',' + xpath_ns_path + ')',
            xpath_normalize_space  = 'normalize-space()',
            xpath_internal         = '[not(' + xpath_has_protocal + ') or ' + xpath_is_internal + ']',
            xpath_external         = '[' + xpath_has_protocal + ' and not(' + xpath_is_internal + ')]',
            escape_literal         = _BAS_SAFE(String.fromCharCode)(30),
            escape_parens          = _BAS_SAFE(String.fromCharCode)(31),
            regex_string_literal   = /("[^"\x1E]*"|'[^'\x1E]*'|=\s*[^\s\]\'\"]+)/g,
            regex_escaped_literal  = /['"]?(\x1E+)['"]?/g,
            regex_css_wrap_pseudo  = /(\x1F\)|[^\)])\:(first|limit|last|gt|lt|eq|nth)([^\-]|$)/,
            regex_specal_chars     = /[\x1C-\x1F]+/g,
            regex_first_axis       = /^([\s\(\x1F]*)(\.?[^\.\/\(]{1,2}[a-z]*:*)/,
            regex_filter_prefix    = /(^|\/|\:)\[/g,
            regex_attr_prefix      = /([^\(\[\/\|\s\x1F])\@/g,
            regex_nth_equation     = /^([-0-9]*)n.*?([0-9]*)$/,
            css_combinators_regex  = /\s*(!?[+>~,^ ])\s*(\.?\/+|[a-z\-]+::)?([a-z\-]+\()?((and\s*|or\s*|mod\s*)?[^+>~,\s'"\]\|\^\$\!\<\=\x1C-\x1F]+)?/g,
            css_combinators_callback = function (match, operator, axis, func, literal, exclude, offset, orig) {
              var prefix = ''; // If we can, we'll prefix a '.'
      
              // XPath operators can look like node-name selectors
              // Detect false positive for " and", " or", " mod"
              if (operator === ' ' && exclude !== undefined) {
                return match;
              }
      
              if (axis === undefined) {
                // Only allow node-selecting XPath functions
                // Detect false positive for " + count(...)", " count(...)", " > position()", etc.
                if (func !== undefined && (func !== 'node(' && func !== 'text(' && func !== 'comment('))                {
                  return;
                } else if (literal === undefined) {
                  literal = func;
                } // Handle case " + text()", " > comment()", etc. where "func" is our "literal"
      
                  // XPath math operators match some CSS combinators
                  // Detect false positive for " + 1", " > 1", etc.
                if (isNumeric(literal)) {
                  return match;
                }
      
                var prevChar = _BAS_SAFE($String.charAt)(orig, offset - 1);
      
                if (prevChar.length === 0 ||
                      prevChar === '(' ||
                      prevChar === '|' ||
                      prevChar === ':') {
                  prefix = '.';
                }
              }
      
              // Return if we don't have a selector to follow the axis
              if (literal === undefined) {
                if (offset + match.length === orig.length) {
                  literal = '*';
                } else {
                  return match;
                }
              }
      
      
              switch (operator) {
              case ' ':
                return '//' + literal;
              case '>':
                return '/' + literal;
              case '+':
                return prefix + '/following-sibling::*[1]/self::' + literal;
              case '~':
                return prefix + '/following-sibling::' + literal;
              case ',':
                if (axis === undefined) {
      
                }
                axis = './/';
                return '|' + axis + literal;
              case '^': // first child
                return '/child::*[1]/self::' + literal;
              case '!^': // last child
                return '/child::*[last()]/self::' + literal;
              case '! ': // ancestor-or-self
                return '/ancestor-or-self::' + literal;
              case '!>': // direct parent
                return '/parent::' + literal;
              case '!+': // adjacent preceding sibling
                return '/preceding-sibling::*[1]/self::' + literal;
              case '!~': // preceding sibling
                return '/preceding-sibling::' + literal;
                  // case '~~'
                  // return '/following-sibling::*/self::|'+selectorStart(orig, offset)+'/preceding-sibling::*/self::'+literal;
              }
            },
      
            css_attributes_regex = /\[([^\@\|\*\=\^\~\$\!\(\/\s\x1C-\x1F]+)\s*(([\|\*\~\^\$\!]?)=?\s*(\x1E+))?\]/g,
            css_attributes_callback = function (str, attr, comp, op, val, offset, orig) {
              var axis = '';
              var prevChar = _BAS_SAFE($String.charAt)(orig, offset - 1);
      
              /*
              if (prevChar === '/' || // found after an axis shortcut ("/", "//", etc.)
                  prevChar === ':')   // found after an axis ("self::", "parent::", etc.)
                  axis = '*';*/
      
              switch (op) {
              case '!':
                return axis + '[not(@' + attr + ') or @' + attr + '!="' + val + '"]';
              case '$':
                return axis + '[substring(@' + attr + ',string-length(@' + attr + ')-(string-length("' + val + '")-1))="' + val + '"]';
              case '^':
                return axis + '[starts-with(@' + attr + ',"' + val + '")]';
              case '~':
                return axis + '[contains(concat(" ",normalize-space(@' + attr + ')," "),concat(" ","' + val + '"," "))]';
              case '*':
                return axis + '[contains(@' + attr + ',"' + val + '")]';
              case '|':
                return axis + '[@' + attr + '="' + val + '" or starts-with(@' + attr + ',concat("' + val + '","-"))]';
              default:
                if (comp === undefined) {
                  if (_BAS_SAFE($String.charAt)(attr, attr.length - 1) === '(' || _BAS_SAFE($String.search)(attr, /^[0-9]+$/) !== -1 || _BAS_SAFE($String.indexOf)(attr, ':') !== -1)                        {
                    return str;
                  }
                  return axis + '[@' + attr + ']';
                } else {
                  return axis + '[@' + attr + '="' + val + '"]';
                }
              }
            },
      
            css_pseudo_classes_regex = /:([a-z\-]+)(\((\x1F+)(([^\x1F]+(\3\x1F+)?)*)(\3\)))?/g,
            css_pseudo_classes_callback = function (match, name, g1, g2, arg, g3, g4, g5, offset, orig) {
              if (_BAS_SAFE($String.charAt)(orig, offset - 1) === ':' && _BAS_SAFE($String.charAt)(orig, offset - 2) !== ':') {
                  // XPath "axis::node-name" will match
                  // Detect false positive ":node-name"
                return match;
              }
      
              if (name === 'odd' || name === 'even') {
                arg  = name;
                name = 'nth-of-type';
              }
      
              switch (name) { // name.toLowerCase()?
              case 'after':
                return '[count(' + css2xpath('preceding::' + arg, true) + ') > 0]';
              case 'after-sibling':
                return '[count(' + css2xpath('preceding-sibling::' + arg, true) + ') > 0]';
              case 'before':
                return '[count(' + css2xpath('following::' + arg, true) + ') > 0]';
              case 'before-sibling':
                return '[count(' + css2xpath('following-sibling::' + arg, true) + ') > 0]';
              case 'checked':
                return '[@selected or @checked]';
              case 'contains':
                return '[contains(' + xpath_normalize_space + ',' + arg + ')]';
              case 'icontains':
                return '[contains(' + xpath_lower_case + ',' + xpath_to_lower(arg) + ')]';
              case 'empty':
                return '[not(*) and not(normalize-space())]';
              case 'enabled':
              case 'disabled':
                return '[@' + name + ']';
              case 'first-child':
                return '[not(preceding-sibling::*)]';
              case 'first':
              case 'limit':
              case 'first-of-type':
                if (arg !== undefined)                    {
                  return '[position()<=' + arg + ']';
                }
                return '[1]';
              case 'gt':
                      // Position starts at 0 for consistency with Sizzle selectors
                return '[position()>' + (_BAS_SAFE(Window.parseInt)(arg, 10) + 1) + ']';
              case 'lt':
                      // Position starts at 0 for consistency with Sizzle selectors
                return '[position()<' + (_BAS_SAFE(Window.parseInt)(arg, 10) + 1) + ']';
              case 'last-child':
                return '[not(following-sibling::*)]';
              case 'only-child':
                return '[not(preceding-sibling::*) and not(following-sibling::*)]';
              case 'only-of-type':
                return '[not(preceding-sibling::*[name()=name(self::node())]) and not(following-sibling::*[name()=name(self::node())])]';
              case 'nth-child':
                if (isNumeric(arg))                    {
                  return '[(count(preceding-sibling::*)+1) = ' + arg + ']';
                }
                switch (arg) {
                case 'even':
                  return '[(count(preceding-sibling::*)+1) mod 2=0]';
                case 'odd':
                  return '[(count(preceding-sibling::*)+1) mod 2=1]';
                default:
                  var a = _BAS_SAFE($String.split)(_BAS_SAFE($String.replace)(arg || '0', regex_nth_equation, '$1+$2'), '+');
      
                  a[0] = a[0] || '1';
                  a[1] = a[1] || '0';
                  return '[(count(preceding-sibling::*)+1)>=' + a[1] + ' and ((count(preceding-sibling::*)+1)-' + a[1] + ') mod ' + a[0] + '=0]';
                }
              case 'nth-of-type':
                if (isNumeric(arg))                    {
                  return '[' + arg + ']';
                }
                switch (arg) {
                case 'odd':
                  return '[position() mod 2=1]';
                case 'even':
                  return '[position() mod 2=0 and position()>=0]';
                default:
                  var a = _BAS_SAFE($String.split)(_BAS_SAFE($String.replace)(arg || '0', regex_nth_equation, '$1+$2'), '+');
      
                  a[0] = a[0] || '1';
                  a[1] = a[1] || '0';
                  return '[position()>=' + a[1] + ' and (position()-' + a[1] + ') mod ' + a[0] + '=0]';
                }
              case 'eq':
              case 'nth':
                // Position starts at 0 for consistency with Sizzle selectors
                if (isNumeric(arg)) {
                  return '[' + (_BAS_SAFE(Window.parseInt)(arg, 10) + 1) + ']';
                }
      
                return '[1]';
              case 'text':
                return '[@type="text"]';
              case 'istarts-with':
                return '[starts-with(' + xpath_lower_case + ',' + xpath_to_lower(arg) + ')]';
              case 'starts-with':
                return '[starts-with(' + xpath_normalize_space + ',' + arg + ')]';
              case 'iends-with':
                return '[' + xpath_ends_with(xpath_lower_case, xpath_to_lower(arg)) + ']';
              case 'ends-with':
                return '[' + xpath_ends_with(xpath_normalize_space, arg) + ']';
              case 'has':
                var xpath = prependAxis(css2xpath(arg, true), './/');
      
                return '[count(' + xpath + ') > 0]';
              case 'has-sibling':
                var xpath = css2xpath('preceding-sibling::' + arg, true);
      
                return '[count(' + xpath + ') > 0 or count(following-sibling::' + _BAS_SAFE($String.substr)(xpath, 19) + ') > 0]';
              case 'has-parent':
                return '[count(' + css2xpath('parent::' + arg, true) + ') > 0]';
              case 'has-ancestor':
                return '[count(' + css2xpath('ancestor::' + arg, true) + ') > 0]';
              case 'last':
              case 'last-of-type':
                if (arg !== undefined)                    {
                  return '[position()>last()-' + arg + ']';
                }
                return '[last()]';
              case 'selected': // Sizzle: "(option) elements that are currently selected"
                return '[local-name()="option" and @selected]';
              case 'skip':
              case 'skip-first':
                return '[position()>' + arg + ']';
              case 'skip-last':
                if (arg !== undefined)                    {
                  return '[last()-position()>=' + arg + ']';
                }
                return '[position()<last()]';
              case 'root':
                return '/ancestor::[last()]';
              case 'range':
                var arr = _BAS_SAFE($String.split)(arg, ',');
      
                return '[' + arr[0] + '<=position() and position()<=' + arr[1] + ']';
              case 'input': // Sizzle: "input, button, select, and textarea are all considered to be input elements."
                return '[local-name()="input" or local-name()="button" or local-name()="select" or local-name()="textarea"]';
              case 'internal':
                return xpath_internal;
              case 'external':
                return xpath_external;
              case 'http':
              case 'https':
              case 'mailto':
              case 'javascript':
                return '[starts-with(@href,concat("' + name + '",":"))]';
              case 'domain':
                return '[(string-length(' + xpath_url_domain() + ')=0 and contains(' + xpath_url_domain(xpath_ns_uri) + ',' + arg + ')) or contains(' + xpath_url_domain() + ',' + arg + ')]';
              case 'path':
                return '[starts-with(' + xpath_url_path() + ',substring-after("' + arg + '","/"))]'
              case 'not':
                var xpath = css2xpath(arg, true);
      
                if (_BAS_SAFE($String.charAt)(xpath, 0) === '[')                    {
                  xpath = 'self::node()' + xpath;
                }
                return '[not(' + xpath + ')]';
              case 'target':
                return '[starts-with(@href, "#")]';
              case 'root':
                return 'ancestor-or-self::*[last()]';
                  /* case 'active':
                  case 'focus':
                  case 'hover':
                  case 'link':
                  case 'visited':
                      return '';*/
              case 'lang':
                return '[@lang="' + arg + '"]';
              case 'read-only':
              case 'read-write':
                return '[@' + _BAS_SAFE($String.replace)(name, '-', '') + ']';
              case 'valid':
              case 'required':
              case 'in-range':
              case 'out-of-range':
                return '[@' + name + ']';
              default:
                return match;
              }
            },
      
            css_ids_classes_regex = /(#|\.)([^\#\@\.\/\(\[\)\]\|\:\s\+\>\<\'\"\x1D-\x1F]+)/g,
            css_ids_classes_callback = function (str, op, val, offset, orig) {
              var axis = '';
              /* var prevChar = _BAS_SAFE($String.charAt)(orig, offset - 1);
              if (prevChar.length === 0 ||
                  prevChar === '/' ||
                  prevChar === '(')
                  axis = '*';
              else if (prevChar === ':')
                  axis = 'node()';*/
              if (op === '#')            {
                return axis + '[@id="' + val + '"]';
              }
              return axis + '[contains(concat(" ",normalize-space(@class)," ")," ' + val + ' ")]';
            };
      
          // Prepend descendant-or-self if no other axis is specified
        function prependAxis(s, axis) {
          return _BAS_SAFE($String.replace)(s, regex_first_axis, function (match, start, literal) {
            if (_BAS_SAFE($String.substr)(literal, literal.length - 2) === '::') // Already has axis::
                  {
              return match;
            }
      
            if (_BAS_SAFE($String.charAt)(literal, 0) === '[')            {
              axis += '*';
            }
              // else if (_BAS_SAFE($String.charAt)(axis, axis.length - 1) === ')')
              //    axis += '/';
            return start + axis + literal;
          });
        }
      
          // Find the begining of the selector, starting at i and working backwards
        function selectorStart(s, i) {
          var depth = 0;
          var offset = 0;
      
          while (i--) {
            switch (_BAS_SAFE($String.charAt)(s, i)) {
            case ' ':
            case escape_parens:
              offset++;
              break;
            case '[':
            case '(':
              depth--;
      
              if (depth < 0)                    {
                return ++i + offset;
              }
              break;
            case ']':
            case ')':
              depth++;
              break;
            case ',':
            case '|':
              if (depth === 0)                    {
                return ++i + offset;
              }
            default:
              offset = 0;
            }
          }
      
          return 0;
        }
      
          // Check if string is numeric
        function isNumeric(s) {
          var num = _BAS_SAFE(Window.parseInt)(s, 10);
      
          return (num === num && '' + num === s);
        }
      
          // Append escape "char" to "open" or "close"
        function escapeChar(s, open, close, char) {
          var depth = 0;
      
          return _BAS_SAFE($String.replace)(s, new _BAS_SAFE(RegExp)('[\\' + open + '\\' + close + ']', 'g'), function (a) {
            if (a === open)            {
              depth++;
            }
      
            if (a === open) {
              return a + repeat(char, depth);
            } else {
              return repeat(char, depth--) + a;
            }
          })
        }
      
        function repeat(str, num) {
          var result = '';
      
          while (true) {
            if (num & 1)            {
              result += str;
            }
            num >>>= 1;
      
            if (num <= 0) {
              break;
            }
            str += str;
          }
      
          return result;
        }
      
        function css2xpath(s, nested) {
          // s = _BAS_SAFE($String.trim)(s);
      
          if (nested === true) {
              // Replace :pseudo-classes
            s = _BAS_SAFE($String.replace)(s, css_pseudo_classes_regex, css_pseudo_classes_callback);
      
              // Replace #ids and .classes
            s = _BAS_SAFE($String.replace)(s, css_ids_classes_regex, css_ids_classes_callback);
      
            return s;
          }
      
          // Tag open and close parenthesis pairs (for RegExp searches)
          s = escapeChar(s, '(', ')', escape_parens);
      
          // Remove and save any string literals
          var literals = [];
      
          s = _BAS_SAFE($String.replace)(s, regex_string_literal, function (s, a) {
            if (_BAS_SAFE($String.charAt)(a, 0) === '=') {
              a = _BAS_SAFE($String.trim)(_BAS_SAFE($String.substr)(a, 1));
      
              if (isNumeric(a))                {
                return s;
              }
            } else {
              a = _BAS_SAFE($String.substr)(a, 1, a.length - 2);
            }
      
            return repeat(escape_literal, _BAS_SAFE($Array.push)(literals, a));
          });
      
          // Replace CSS combinators (" ", "+", ">", "~", ",") and reverse combinators ("!", "!+", "!>", "!~")
          s = _BAS_SAFE($String.replace)(s, css_combinators_regex, css_combinators_callback);
      
          // Replace CSS attribute filters
          s = _BAS_SAFE($String.replace)(s, css_attributes_regex, css_attributes_callback);
      
          // Wrap certain :pseudo-classes in parens (to collect node-sets)
          while (true) {
            var index = _BAS_SAFE($String.search)(s, regex_css_wrap_pseudo);
      
            if (index === -1) {
              break;
            }
            index = _BAS_SAFE($String.indexOf)(s, ':', index);
            var start = selectorStart(s, index);
      
            s = _BAS_SAFE($String.substr)(s, 0, start) +
                  '(' + _BAS_SAFE($String.substring)(s, start, index) + ')' +
                  _BAS_SAFE($String.substr)(s, index);
          }
      
          // Replace :pseudo-classes
          s = _BAS_SAFE($String.replace)(s, css_pseudo_classes_regex, css_pseudo_classes_callback);
      
          // Replace #ids and .classes
          s = _BAS_SAFE($String.replace)(s, css_ids_classes_regex, css_ids_classes_callback);
      
          // Restore the saved string literals
          s = _BAS_SAFE($String.replace)(s, regex_escaped_literal, function (s, a) {
            var str = literals[a.length - 1];
      
            return '"' + str + '"';
          })
      
          // Remove any special characters
          s = _BAS_SAFE($String.replace)(s, regex_specal_chars, '');
      
          // add * to stand-alone filters
          s = _BAS_SAFE($String.replace)(s, regex_filter_prefix, '$1*[');
      
          // add "/" between @attribute selectors
          s = _BAS_SAFE($String.replace)(s, regex_attr_prefix, '$1/@');
      
          /*
          Combine multiple filters?
          s = escapeChar(s, '[', ']', filter_char);
          s = _BAS_SAFE($String.replace)(s, /(\x1D+)\]\[\1(.+?[^\x1D])\1\]/g, ' and ($2)$1]')
          */
      
          s = prependAxis(s, './/'); // prepend ".//" axis to begining of CSS selector
          return s;
        }
      
      
        return css2xpath;
      
      })();
      
  
    };
  
_BAS_HIDE(BrowserAutomationStudio_GenerateMultiSelectors) = function(include,exclude,selector_prefix,shadow_prefix) {
    var _query_selector_host = document;
    if(include.length > 0)
      _query_selector_host = _BAS_HIDE(BrowserAutomationStudio_GetQuerySelectorHost)(include[0]);
    else if(exclude.length > 0)
      _query_selector_host = _BAS_HIDE(BrowserAutomationStudio_GetQuerySelectorHost)(exclude[0]);

    var CssResultsRaw = []
    var CssResults = []
    var XpathResults = []

    var SelectInputs = [
      [false,true],
      [false,false],
      [true,true]
    ]
    for(var i = 0;i<SelectInputs.length;i++)
    {
      var Input = SelectInputs[i]
      var Helper = new _BAS_HIDE(DomPredictionHelper)(Input[0], Input[1], false, _query_selector_host)
      var Selector = Helper.predictCss(include,exclude)
      var Density = Helper.getQueryDensity(Selector)
      if(Density > 5)
      {
        Helper = new _BAS_HIDE(DomPredictionHelper)(Input[0], Input[1], true, _query_selector_host)
        Selector = Helper.predictCss(include,exclude)
      }
      _BAS_SAFE($Array.push)(CssResultsRaw, Selector)
    }
    
    for(var i = 0;i<CssResultsRaw.length;i++)
    {
        var Result = CssResultsRaw[i]
        var ToAdd = selector_prefix + shadow_prefix + ">CSS>" + Result
        if(Result && _BAS_SAFE($Array.indexOf)(CssResults, ToAdd) < 0)
          _BAS_SAFE($Array.push)(CssResults, ToAdd)

        if(Result && XpathResults.length == 0)
        {
          var ResultXpath = "";

          try
          {
            ResultXpath = new _BAS_HIDE(DomPredictionHelper)(false, true, false, _query_selector_host).css2xpath(Result);
          }catch(e)
          {
            ResultXpath = ""
          }

          if(!ResultXpath)
          {
            try
            {
              ResultXpath = new _BAS_HIDE(DomPredictionHelper)(false, true, false, _query_selector_host).cssToXPath(Result);
            }catch(e)
            {
              ResultXpath = ""
            }
          }
          
          if(ResultXpath)
            _BAS_SAFE($Array.push)(XpathResults, selector_prefix + shadow_prefix + ">XPATH>" + ResultXpath)
        }
        
    }
    
    return {
        css: CssResults,
        xpath: XpathResults
    }

};