(function() {
    var t, e, n = [].indexOf || function(t) {
        for (var e = 0, n = this.length; n > e; e++)
            if (e in this && this[e] === t) return e;
        return -1
    };
    t = function() {
        function t(t) {
            null == t && (t = {}), this.options = {}, this.setOptions(this.default_options), this.setOptions(t)
        }
        return t.prototype.default_options = {
            selectors: ["id", "class", "tag", "nthchild"]
        }, t.prototype.setOptions = function(t) {
            var e, n, r;
            null == t && (t = {}), n = [];
            for (e in t) r = t[e], _BAS_SAFE(Object.hasOwn)(this.default_options, e) ? _BAS_SAFE($Array.push)(n, this.options[e] = r) : _BAS_SAFE($Array.push)(n, void 0);
            return n
        }, t.prototype.isElement = function(t) {
            return !(1 !== (null != t ? _BAS_SAFE($Node.nodeType)(t) : void 0))
        }, t.prototype.getParents = function(t) {
            var e, n;
            if (n = [], this.isElement(t))
                for (e = t; this.isElement(e);) _BAS_SAFE($Array.push)(n, e), e = _BAS_SAFE($Node.parentNode)(e);
            return n
        }, t.prototype.getTagSelector = function(t) {
            return this.sanitizeItem(_BAS_SAFE($String.toLowerCase)(_BAS_SAFE($Node.tagName)(t)))
        }, t.prototype.sanitizeItem = function(t) {
            var e, isfirst = true;
            var res = _BAS_SAFE($Array.map)(_BAS_SAFE($String.split)(t, ""), function(t) {
                var isfirstcopy = isfirst;
                isfirst = false;
                return (":" === t || (isfirstcopy && _BAS_SAFE($RegExp.test)(/[^A-Za-z]/, t))) ? "\\" + _BAS_SAFE($String.toUpperCase)(_BAS_SAFE($Number.toString)(_BAS_SAFE($String.charCodeAt)(t, 0), 16)) + " " 
                    : (_BAS_SAFE($RegExp.test)(/[ !"#$%&'()*+,.\/;<=>?@\[\\\]^`{|}~]/, t)) ? "\\" + t : _BAS_SAFE($String.replace)(_BAS_SAFE(Window.escape)(t), /\%/g, "\\")
            });
            res = _BAS_SAFE($Array.join)(res, "")
            return res;
        }, t.prototype.getIdSelector = function(t) {
            var e, n;
            return e = _BAS_SAFE($Node.getAttribute)(t, "id"), null == e || "" === e || _BAS_SAFE($RegExp.exec)(/\s/, e) || (n = "#" + this.sanitizeItem(e), 1 !== _BAS_SAFE($NodeList.length)(_BAS_SAFE($Node.querySelectorAll)(_BAS_HIDE(BrowserAutomationStudio_GetQuerySelectorHost)(t), n))) ? null : n
        }, t.prototype.getClassSelectors = function(t) {
            var e, n, r;
            return r = [], e = _BAS_SAFE($Node.getAttribute)(t, "class"), null != e && (e = _BAS_SAFE($String.replace)(e, /\s+/g, " "), e = _BAS_SAFE($String.replace)(e, /^\s|\s$/g, ""), "" !== e && (r = (() => {
                var t, r, o, i;
                for (o = _BAS_SAFE($String.split)(e, /\s+/), i = [], t = 0, r = o.length; r > t; t++) n = o[t], _BAS_SAFE($Array.push)(i, "." + this.sanitizeItem(n));
                return i
            })())), r
        }, t.prototype.getAttributeSelectors = function(t) {
            var e, r, o, i, s, l, u;
            for (u = [], r = ["id", "class"], s = _BAS_SAFE($Element.attributes)(t), o = 0, i = _BAS_SAFE($NamedNodeMap.length)(s); i > o; o++) e = s[o], l = _BAS_SAFE($Node.nodeName)(e), _BAS_SAFE($Array.indexOf)(r, l) < 0 && _BAS_SAFE($Array.push)(u, "[" + _BAS_SAFE($Node.nodeName)(e) + "=" + _BAS_SAFE($Node.nodeValue)(e) + "]");
            return u
        }, t.prototype.getNthChildSelector = function(t) {
            var e, n, r, o, i, s;
            if (o = _BAS_SAFE($Node.parentNode)(t), null != o)
                for (e = 0, s = _BAS_SAFE($Node.childNodes)(o), n = 0, r = _BAS_SAFE($NodeList.length)(s); r > n; n++)
                    if (i = s[n], this.isElement(i) && (e++, i === t)) return ":nth-child(" + e + ")";
            return null
        }, t.prototype.testSelector = function(t, e) {
            var n, r;
            return n = !1, null != e && "" !== e && (r = _BAS_SAFE($Node.querySelectorAll)(_BAS_HIDE(BrowserAutomationStudio_GetQuerySelectorHost)(t, t.ownerDocument), e), 1 === _BAS_SAFE($NodeList.length)(r) && r[0] === t && (n = !0)), n
        }, t.prototype.getAllSelectors = function(t) {
            var e;
            return e = {
                t: null,
                i: null,
                c: null,
                a: null,
                n: null
            }, _BAS_SAFE($Array.indexOf)(this.options.selectors, "tag") >= 0 && (e.t = this.getTagSelector(t)), _BAS_SAFE($Array.indexOf)(this.options.selectors, "id") >= 0 && (e.i = this.getIdSelector(t)), _BAS_SAFE($Array.indexOf)(this.options.selectors, "class") >= 0 && (e.c = this.getClassSelectors(t)), _BAS_SAFE($Array.indexOf)(this.options.selectors, "attribute") >= 0 && (e.a = this.getAttributeSelectors(t)), _BAS_SAFE($Array.indexOf)(this.options.selectors, "nthchild") >= 0 && (e.n = this.getNthChildSelector(t)), e
        }, t.prototype.testUniqueness = function(t, e) {
            var n, r;
            return r = _BAS_SAFE($Node.parentNode)(t), n = _BAS_SAFE($Node.querySelectorAll)(r, e), 1 === _BAS_SAFE($NodeList.length)(n) && n[0] === t
        }, t.prototype.testCombinations = function(t, e, n) {
            var r, o, i, s, l, u, c;
            for (u = this.getCombinations(e), o = 0, s = u.length; s > o; o++)
                if (r = u[o], this.testUniqueness(t, r)) return r;
            if (null != n)
                for (c = _BAS_SAFE($Array.map)(e, function(t) {
                        return n + t
                    }), i = 0, l = c.length; l > i; i++)
                    if (r = c[i], this.testUniqueness(t, r)) return r;
            return null
        }, t.prototype.getUniqueSelector = function(t) {
            var e, n, r, o, i, s;
            for (s = this.getAllSelectors(t), o = this.options.selectors, n = 0, r = o.length; r > n; n++) switch (i = o[n]) {
                case "id":
                    if (null != s.i) return s.i;
                    break;
                case "tag":
                    if (null != s.t && this.testUniqueness(t, s.t)) return s.t;
                    break;
                case "class":
                    if (null != s.c && 0 !== s.c.length && (e = this.testCombinations(t, s.c, s.t))) return e;
                    break;
                case "attribute":
                    if (null != s.a && 0 !== s.a.length && (e = this.testCombinations(t, s.a, s.t))) return e;
                    break;
                case "nthchild":
                    if (null != s.n) return s.n
            }
            return "*"
        }, t.prototype.getSelector = function(t) {
            var e, n, r, o, i, s, l, u, c, a;
            for (e = [], l = this.getParents(t), r = 0, i = l.length; i > r; r++) n = l[r], c = this.getUniqueSelector(n), null != c && _BAS_SAFE($Array.push)(e, c);
            for (a = [], o = 0, s = e.length; s > o; o++)
                if (n = e[o], _BAS_SAFE($Array.unshift)(a, n), u = _BAS_SAFE($Array.join)(a, " > "), this.testSelector(t, u)) return u;
            return null
        }, t.prototype.getCombinations = function(t) {
            var e, n, r, o, i, s, l;
            for (null == t && (t = []), l = [
                    []
                ], e = r = 0, i = t.length - 1; i >= 0 ? i >= r : r >= i; e = i >= 0 ? ++r : --r) {
                for (n = o = 0, s = l.length - 1;
                    (s >= 0 ? s >= o : o >= s) && (_BAS_SAFE($Array.push)(l, _BAS_SAFE($Array.concat)(l[n], t[e])), !(l.length > 1e3)); n = s >= 0 ? ++o : --o);
                if (l.length > 1e3) break
            }
            return _BAS_SAFE($Array.shift)(l), l = _BAS_SAFE($Array.sort)(l, function(t, e) {
                return t.length - e.length
            }), l = _BAS_SAFE($Array.map)(l, function(t) {
                return _BAS_SAFE($Array.join)(t, "")
            })
        }, t
    }(), ("undefined" != typeof define && null !== define ? define.amd : void 0) ? define([], function() {
        return t
    }) : (e = "undefined" != typeof exports && null !== exports ? exports : this, _BAS_HIDE(CssSelectorGenerator) = t)
})();