(function () {
  _BAS_HIDE(diff_match_patch) = function () {
    this.Diff_Timeout = 1.0;
    this.Diff_EditCost = 4;
    this.Diff_DualThreshold = 32;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_main = function (a, b, c) {
    if (a == b) {
      return [[0, a]];
    }
    if (typeof c == 'undefined') {
      c = true;
    }
    var d = c;
    var e = this.diff_commonPrefix(a, b);
    var f = _BAS_SAFE($String.substring)(a, 0, e);
    a = _BAS_SAFE($String.substring)(a, e);
    b = _BAS_SAFE($String.substring)(b, e);
    e = this.diff_commonSuffix(a, b);
    var g = _BAS_SAFE($String.substring)(a, a.length - e);
    a = _BAS_SAFE($String.substring)(a, 0, a.length - e);
    b = _BAS_SAFE($String.substring)(b, 0, b.length - e);
    var h = this.diff_compute(a, b, d);
    if (f) {
      _BAS_SAFE($Array.unshift)(h, [0, f]);
    }
    if (g) {
      _BAS_SAFE($Array.push)(h, [0, g]);
    }
    this.diff_cleanupMerge(h);
    return h;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_compute = function (b, c, d) {
    var e;
    if (!b) {
      return [[1, c]];
    }
    if (!c) {
      return [[-1, b]];
    }
    var f = b.length > c.length ? b : c;
    var g = b.length > c.length ? c : b;
    var i = _BAS_SAFE($String.indexOf)(f, g);
    if (i != -1) {
      e = [
        [1, _BAS_SAFE($String.substring)(f, 0, i)],
        [0, g],
        [1, _BAS_SAFE($String.substring)(f, i + g.length)],
      ];
      if (b.length > c.length) {
        e[0][0] = e[2][0] = -1;
      }
      return e;
    }
    f = g = null;
    var h = this.diff_halfMatch(b, c);
    if (h) {
      var k = h[0];
      var l = h[1];
      var m = h[2];
      var n = h[3];
      var o = h[4];
      var p = this.diff_main(k, m, d);
      var q = this.diff_main(l, n, d);
      return _BAS_SAFE($Array.concat)(p, [[0, o]], q);
    }
    if (d && (b.length < 100 || c.length < 100)) {
      d = false;
    }
    var r;
    if (d) {
      var a = this.diff_linesToChars(b, c);
      b = a[0];
      c = a[1];
      r = a[2];
    }
    e = this.diff_map(b, c);
    if (!e) {
      e = [
        [-1, b],
        [1, c],
      ];
    }
    if (d) {
      this.diff_charsToLines(e, r);
      this.diff_cleanupSemantic(e);
      _BAS_SAFE($Array.push)(e, [0, '']);
      var s = 0;
      var t = 0;
      var u = 0;
      var v = '';
      var w = '';
      while (s < e.length) {
        switch (e[s][0]) {
          case 1:
            u++;
            w += e[s][1];
            break;
          case -1:
            t++;
            v += e[s][1];
            break;
          case 0:
            if (t >= 1 && u >= 1) {
              var a = this.diff_main(v, w, false);
              _BAS_SAFE($Array.splice)(e, s - t - u, t + u);
              s = s - t - u;
              for (var j = a.length - 1; j >= 0; j--) {
                _BAS_SAFE($Array.splice)(e, s, 0, a[j]);
              }
              s = s + a.length;
            }
            u = 0;
            t = 0;
            v = '';
            w = '';
            break;
        }
        s++;
      }
      _BAS_SAFE($Array.pop)(e);
    }
    return e;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_linesToChars = function (g, h) {
    var i = [];
    var j = {};
    i[0] = '';
    function diff_linesToCharsMunge(a) {
      var b = '';
      var c = 0;
      var d = -1;
      var e = i.length;
      while (d < a.length - 1) {
        d = _BAS_SAFE($String.indexOf)(a, '\n', c);
        if (d == -1) {
          d = a.length - 1;
        }
        var f = _BAS_SAFE($String.substring)(a, c, d + 1);
        c = d + 1;
        if (_BAS_SAFE(Object.hasOwn)(j, f)) {
          b += _BAS_SAFE(String.fromCharCode)(j[f]);
        } else {
          b += _BAS_SAFE(String.fromCharCode)(e);
          j[f] = e;
          i[e++] = f;
        }
      }
      return b;
    }
    var k = diff_linesToCharsMunge(g);
    var l = diff_linesToCharsMunge(h);
    return [k, l, i];
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_charsToLines = function (a, b) {
    for (var x = 0; x < a.length; x++) {
      var c = a[x][1];
      var d = [];
      for (var y = 0; y < c.length; y++) {
        d[y] = b[_BAS_SAFE($String.charCodeAt)(c, y)];
      }
      a[x][1] = _BAS_SAFE($Array.join)(d, '');
    }
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_map = function (b, c) {
    var e = _BAS_SAFE(Date.now)() + this.Diff_Timeout * 1000;
    var f = b.length + c.length - 1;
    var g = this.Diff_DualThreshold * 2 < f;
    var h = [];
    var i = [];
    var j = {};
    var l = {};
    j[1] = 0;
    l[1] = 0;
    var x, y;
    var m;
    var n = {};
    var o = false;
    var hasOwnProperty = !!n.hasOwnProperty;
    var p = (b.length + c.length) % 2;
    for (var d = 0; d < f; d++) {
      if (this.Diff_Timeout > 0 && _BAS_SAFE(Date.now)() > e) {
        return null;
      }
      h[d] = {};
      for (var k = -d; k <= d; k += 2) {
        if (k == -d || (k != d && j[k - 1] < j[k + 1])) {
          x = j[k + 1];
        } else {
          x = j[k - 1] + 1;
        }
        y = x - k;
        if (g) {
          m = x + ',' + y;
          if (p && _BAS_SAFE(Object.hasOwn)(n, m)) {
            o = true;
          }
          if (!p) {
            n[m] = d;
          }
        }
        while (!o && x < b.length && y < c.length && _BAS_SAFE($String.charAt)(b, x) == _BAS_SAFE($String.charAt)(c, y)) {
          x++;
          y++;
          if (g) {
            m = x + ',' + y;
            if (p && _BAS_SAFE(Object.hasOwn)(n, m)) {
              o = true;
            }
            if (!p) {
              n[m] = d;
            }
          }
        }
        j[k] = x;
        h[d][x + ',' + y] = true;
        if (x == b.length && y == c.length) {
          return this.diff_path1(h, b, c);
        } else if (o) {
          i = _BAS_SAFE($Array.slice)(i, 0, n[m] + 1);
          var a = this.diff_path1(h, _BAS_SAFE($String.substring)(b, 0, x), _BAS_SAFE($String.substring)(c, 0, y));
          return _BAS_SAFE($Array.concat)(a, this.diff_path2(i, _BAS_SAFE($String.substring)(b, x), _BAS_SAFE($String.substring)(c, y)));
        }
      }
      if (g) {
        i[d] = {};
        for (var k = -d; k <= d; k += 2) {
          if (k == -d || (k != d && l[k - 1] < l[k + 1])) {
            x = l[k + 1];
          } else {
            x = l[k - 1] + 1;
          }
          y = x - k;
          m = b.length - x + ',' + (c.length - y);
          if (!p && _BAS_SAFE(Object.hasOwn)(n, m)) {
            o = true;
          }
          if (p) {
            n[m] = d;
          }
          while (!o && x < b.length && y < c.length && _BAS_SAFE($String.charAt)(b, b.length - x - 1) == _BAS_SAFE($String.charAt)(c, c.length - y - 1)) {
            x++;
            y++;
            m = b.length - x + ',' + (c.length - y);
            if (!p && _BAS_SAFE(Object.hasOwn)(n, m)) {
              o = true;
            }
            if (p) {
              n[m] = d;
            }
          }
          l[k] = x;
          i[d][x + ',' + y] = true;
          if (o) {
            h = _BAS_SAFE($Array.slice)(h, 0, n[m] + 1);
            var a = this.diff_path1(h, _BAS_SAFE($String.substring)(b, 0, b.length - x), _BAS_SAFE($String.substring)(c, 0, c.length - y));
            return _BAS_SAFE($Array.concat)(a, this.diff_path2(i, _BAS_SAFE($String.substring)(b, b.length - x), _BAS_SAFE($String.substring)(c, c.length - y)));
          }
        }
      }
    }
    return null;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_path1 = function (a, b, c) {
    var e = [];
    var x = b.length;
    var y = c.length;
    var f = null;
    for (var d = a.length - 2; d >= 0; d--) {
      while (1) {
        if (_BAS_SAFE(Object.hasOwn)(a[d], x - 1 + ',' + y)) {
          x--;
          if (f === -1) {
            e[0][1] = _BAS_SAFE($String.charAt)(b, x) + e[0][1];
          } else {
            _BAS_SAFE($Array.unshift)(e, [-1, _BAS_SAFE($String.charAt)(b, x)]);
          }
          f = -1;
          break;
        } else if (
          _BAS_SAFE(Object.hasOwn)(a[d], x + ',' + (y - 1))
        ) {
          y--;
          if (f === 1) {
            e[0][1] = _BAS_SAFE($String.charAt)(c, y) + e[0][1];
          } else {
            _BAS_SAFE($Array.unshift)(e, [1, _BAS_SAFE($String.charAt)(c, y)]);
          }
          f = 1;
          break;
        } else {
          x--;
          y--;
          if (f === 0) {
            e[0][1] = _BAS_SAFE($String.charAt)(b, x) + e[0][1];
          } else {
            _BAS_SAFE($Array.unshift)(e, [0, _BAS_SAFE($String.charAt)(b, x)]);
          }
          f = 0;
        }
      }
    }
    return e;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_path2 = function (a, b, c) {
    var e = [];
    var f = 0;
    var x = b.length;
    var y = c.length;
    var g = null;
    for (var d = a.length - 2; d >= 0; d--) {
      while (1) {
        if (_BAS_SAFE(Object.hasOwn)(a[d], x - 1 + ',' + y)) {
          x--;
          if (g === -1) {
            e[f - 1][1] += _BAS_SAFE($String.charAt)(b, b.length - x - 1);
          } else {
            e[f++] = [-1, _BAS_SAFE($String.charAt)(b, b.length - x - 1)];
          }
          g = -1;
          break;
        } else if (
          _BAS_SAFE(Object.hasOwn)(a[d], x + ',' + (y - 1))
        ) {
          y--;
          if (g === 1) {
            e[f - 1][1] += _BAS_SAFE($String.charAt)(c, c.length - y - 1);
          } else {
            e[f++] = [1, _BAS_SAFE($String.charAt)(c, c.length - y - 1)];
          }
          g = 1;
          break;
        } else {
          x--;
          y--;
          if (g === 0) {
            e[f - 1][1] += _BAS_SAFE($String.charAt)(b, b.length - x - 1);
          } else {
            e[f++] = [0, _BAS_SAFE($String.charAt)(b, b.length - x - 1)];
          }
          g = 0;
        }
      }
    }
    return e;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_commonPrefix = function (a, b) {
    if (!a || !b || _BAS_SAFE($String.charCodeAt)(a, 0) !== _BAS_SAFE($String.charCodeAt)(b, 0)) {
      return 0;
    }
    var c = 0;
    var d = _BAS_SAFE(Math.min)(a.length, b.length);
    var e = d;
    var f = 0;
    while (c < e) {
      if (_BAS_SAFE($String.substring)(a, f, e) == _BAS_SAFE($String.substring)(b, f, e)) {
        c = e;
        f = c;
      } else {
        d = e;
      }
      e = _BAS_SAFE(Math.floor)((d - c) / 2 + c);
    }
    return e;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_commonSuffix = function (a, b) {
    if (!a || !b || _BAS_SAFE($String.charCodeAt)(a, a.length - 1) !== _BAS_SAFE($String.charCodeAt)(b, b.length - 1)) {
      return 0;
    }
    var c = 0;
    var d = _BAS_SAFE(Math.min)(a.length, b.length);
    var e = d;
    var f = 0;
    while (c < e) {
      if (_BAS_SAFE($String.substring)(a, a.length - e, a.length - f) == _BAS_SAFE($String.substring)(b, b.length - e, b.length - f)) {
        c = e;
        f = c;
      } else {
        d = e;
      }
      e = _BAS_SAFE(Math.floor)((d - c) / 2 + c);
    }
    return e;
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_halfMatch = function (h, k) {
    var l = h.length > k.length ? h : k;
    var m = h.length > k.length ? k : h;
    if (l.length < 10 || m.length < 1) {
      return null;
    }
    var n = this;
    function diff_halfMatchI(a, b, i) {
      var c = _BAS_SAFE($String.substring)(a, i, i + _BAS_SAFE(Math.floor)(a.length / 4));
      var j = -1;
      var d = '';
      var e, best_longtext_b, best_shorttext_a, best_shorttext_b;
      while ((j = _BAS_SAFE($String.indexOf)(b, c, j + 1)) != -1) {
        var f = n.diff_commonPrefix(_BAS_SAFE($String.substring)(a, i), _BAS_SAFE($String.substring)(b, j));
        var g = n.diff_commonSuffix(_BAS_SAFE($String.substring)(a, 0, i), _BAS_SAFE($String.substring)(b, 0, j));
        if (d.length < g + f) {
          d = _BAS_SAFE($String.substring)(b, j - g, j) + _BAS_SAFE($String.substring)(b, j, j + f);
          e = _BAS_SAFE($String.substring)(a, 0, i - g);
          best_longtext_b = _BAS_SAFE($String.substring)(a, i + f);
          best_shorttext_a = _BAS_SAFE($String.substring)(b, 0, j - g);
          best_shorttext_b = _BAS_SAFE($String.substring)(b, j + f);
        }
      }
      if (d.length >= a.length / 2) {
        return [e, best_longtext_b, best_shorttext_a, best_shorttext_b, d];
      } else {
        return null;
      }
    }
    var o = diff_halfMatchI(l, m, _BAS_SAFE(Math.ceil)(l.length / 4));
    var p = diff_halfMatchI(l, m, _BAS_SAFE(Math.ceil)(l.length / 2));
    var q;
    if (!o && !p) {
      return null;
    } else if (!p) {
      q = o;
    } else if (!o) {
      q = p;
    } else {
      q = o[4].length > p[4].length ? o : p;
    }
    var r, text1_b, text2_a, text2_b;
    if (h.length > k.length) {
      r = q[0];
      text1_b = q[1];
      text2_a = q[2];
      text2_b = q[3];
    } else {
      text2_a = q[0];
      text2_b = q[1];
      r = q[2];
      text1_b = q[3];
    }
    var s = q[4];
    return [r, text1_b, text2_a, text2_b, s];
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_cleanupSemantic = function (a) {
    var b = false;
    var c = [];
    var d = 0;
    var e = null;
    var f = 0;
    var g = 0;
    var h = 0;
    while (f < a.length) {
      if (a[f][0] == 0) {
        c[d++] = f;
        g = h;
        h = 0;
        e = a[f][1];
      } else {
        h += a[f][1].length;
        if (e !== null && e.length <= g && e.length <= h) {
          _BAS_SAFE($Array.splice)(a, c[d - 1], 0, [-1, e]);
          a[c[d - 1] + 1][0] = 1;
          d--;
          d--;
          f = d > 0 ? c[d - 1] : -1;
          g = 0;
          h = 0;
          e = null;
          b = true;
        }
      }
      f++;
    }
    if (b) {
      this.diff_cleanupMerge(a);
    }
    this.diff_cleanupSemanticLossless(a);
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_cleanupSemanticLossless = function (d) {
    var e = /[^a-zA-Z0-9]/;
    var f = /\s/;
    var g = /[\r\n]/;
    var h = /\n\r?\n$/;
    var i = /^\r?\n\r?\n/;
    function diff_cleanupSemanticScore(a, b) {
      if (!a || !b) {
        return 5;
      }
      var c = 0;
      if (_BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(a, a.length - 1), e) || _BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(b, 0), e)) {
        c++;
        if (_BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(a, a.length - 1), f) || _BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(b, 0), f)) {
          c++;
          if (_BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(a, a.length - 1), g) || _BAS_SAFE($String.match)(_BAS_SAFE($String.charAt)(b, 0), g)) {
            c++;
            if (_BAS_SAFE($String.match)(a, h) || _BAS_SAFE($String.match)(b, i)) {
              c++;
            }
          }
        }
      }
      return c;
    }
    var j = 1;
    while (j < d.length - 1) {
      if (d[j - 1][0] == 0 && d[j + 1][0] == 0) {
        var k = d[j - 1][1];
        var l = d[j][1];
        var m = d[j + 1][1];
        var n = this.diff_commonSuffix(k, l);
        if (n) {
          var o = _BAS_SAFE($String.substring)(l, l.length - n);
          k = _BAS_SAFE($String.substring)(k, 0, k.length - n);
          l = o + _BAS_SAFE($String.substring)(l, 0, l.length - n);
          m = o + m;
        }
        var p = k;
        var q = l;
        var r = m;
        var s = diff_cleanupSemanticScore(k, l) + diff_cleanupSemanticScore(l, m);
        while (_BAS_SAFE($String.charAt)(l, 0) === _BAS_SAFE($String.charAt)(m, 0)) {
          k += _BAS_SAFE($String.charAt)(l, 0);
          l = _BAS_SAFE($String.substring)(l, 1) + _BAS_SAFE($String.charAt)(m, 0);
          m = _BAS_SAFE($String.substring)(m, 1);
          var t = diff_cleanupSemanticScore(k, l) + diff_cleanupSemanticScore(l, m);
          if (t >= s) {
            s = t;
            p = k;
            q = l;
            r = m;
          }
        }
        if (d[j - 1][1] != p) {
          if (p) {
            d[j - 1][1] = p;
          } else {
            _BAS_SAFE($Array.splice)(d, j - 1, 1);
            j--;
          }
          d[j][1] = q;
          if (r) {
            d[j + 1][1] = r;
          } else {
            _BAS_SAFE($Array.splice)(d, j + 1, 1);
            j--;
          }
        }
      }
      j++;
    }
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_cleanupEfficiency = function (a) {
    var b = false;
    var c = [];
    var d = 0;
    var e = '';
    var f = 0;
    var g = false;
    var h = false;
    var i = false;
    var j = false;
    while (f < a.length) {
      if (a[f][0] == 0) {
        if (a[f][1].length < this.Diff_EditCost && (i || j)) {
          c[d++] = f;
          g = i;
          h = j;
          e = a[f][1];
        } else {
          d = 0;
          e = '';
        }
        i = j = false;
      } else {
        if (a[f][0] == -1) {
          j = true;
        } else {
          i = true;
        }
        if (e && ((g && h && i && j) || (e.length < this.Diff_EditCost / 2 && g + h + i + j == 3))) {
          _BAS_SAFE($Array.splice)(a, c[d - 1], 0, [-1, e]);
          a[c[d - 1] + 1][0] = 1;
          d--;
          e = '';
          if (g && h) {
            i = j = true;
            d = 0;
          } else {
            d--;
            f = d > 0 ? c[d - 1] : -1;
            i = j = false;
          }
          b = true;
        }
      }
      f++;
    }
    if (b) {
      this.diff_cleanupMerge(a);
    }
  };
  _BAS_HIDE(diff_match_patch).prototype.diff_cleanupMerge = function (a) {
    _BAS_SAFE($Array.push)(a, [0, '']);
    var b = 0;
    var c = 0;
    var d = 0;
    var e = '';
    var f = '';
    var g;
    while (b < a.length) {
      switch (a[b][0]) {
        case 1:
          d++;
          f += a[b][1];
          b++;
          break;
        case -1:
          c++;
          e += a[b][1];
          b++;
          break;
        case 0:
          if (c !== 0 || d !== 0) {
            if (c !== 0 && d !== 0) {
              g = this.diff_commonPrefix(f, e);
              if (g !== 0) {
                if (b - c - d > 0 && a[b - c - d - 1][0] == 0) {
                  a[b - c - d - 1][1] += _BAS_SAFE($String.substring)(f, 0, g);
                } else {
                  _BAS_SAFE($Array.splice)(a, 0, 0, [0, _BAS_SAFE($String.substring)(f, 0, g)]);
                  b++;
                }
                f = _BAS_SAFE($String.substring)(f, g);
                e = _BAS_SAFE($String.substring)(e, g);
              }
              g = this.diff_commonSuffix(f, e);
              if (g !== 0) {
                a[b][1] = _BAS_SAFE($String.substring)(f, f.length - g) + a[b][1];
                f = _BAS_SAFE($String.substring)(f, 0, f.length - g);
                e = _BAS_SAFE($String.substring)(e, 0, e.length - g);
              }
            }
            if (c === 0) {
              _BAS_SAFE($Array.splice)(a, b - c - d, c + d, [1, f]);
            } else if (d === 0) {
              _BAS_SAFE($Array.splice)(a, b - c - d, c + d, [-1, e]);
            } else {
              _BAS_SAFE($Array.splice)(a, b - c - d, c + d, [-1, e], [1, f]);
            }
            b = b - c - d + (c ? 1 : 0) + (d ? 1 : 0) + 1;
          } else if (b !== 0 && a[b - 1][0] == 0) {
            a[b - 1][1] += a[b][1];
            _BAS_SAFE($Array.splice)(a, b, 1);
          } else {
            b++;
          }
          d = 0;
          c = 0;
          e = '';
          f = '';
          break;
      }
    }
    if (a[a.length - 1][1] === '') {
      _BAS_SAFE($Array.pop)(a);
    }
    var h = false;
    b = 1;
    while (b < a.length - 1) {
      if (a[b - 1][0] == 0 && a[b + 1][0] == 0) {
        if (_BAS_SAFE($String.substring)(a[b][1], a[b][1].length - a[b - 1][1].length) == a[b - 1][1]) {
          a[b][1] = a[b - 1][1] + _BAS_SAFE($String.substring)(a[b][1], 0, a[b][1].length - a[b - 1][1].length);
          a[b + 1][1] = a[b - 1][1] + a[b + 1][1];
          _BAS_SAFE($Array.splice)(a, b - 1, 1);
          h = true;
        } else if (_BAS_SAFE($String.substring)(a[b][1], 0, a[b + 1][1].length) == a[b + 1][1]) {
          a[b - 1][1] += a[b + 1][1];
          a[b][1] = _BAS_SAFE($String.substring)(a[b][1], a[b + 1][1].length) + a[b + 1][1];
          _BAS_SAFE($Array.splice)(a, b + 1, 1);
          h = true;
        }
      }
      b++;
    }
    if (h) {
      this.diff_cleanupMerge(a);
    }
  };
})();
