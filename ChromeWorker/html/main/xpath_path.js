_BAS_HIDE(BrowserAutomationStudio_CreateXPathFromElement) = function (elm) {
  const $join = _BAS_SAFE($Array.join);
  const $unshift = _BAS_SAFE($Array.unshift);
  const $localName = _BAS_SAFE($Node.localName);
  const $toLowerCase = _BAS_SAFE($String.toLowerCase);
  var allNodes = _BAS_SAFE($Node.querySelectorAll)(_BAS_HIDE(BrowserAutomationStudio_GetQuerySelectorHost)(elm), '*');
  for (var segs = []; elm && _BAS_SAFE($Node.nodeType)(elm) === 1; elm = _BAS_SAFE($Node.parentNode)(elm)) {
    if (_BAS_SAFE($Node.hasAttribute)(elm, 'id')) {
      var uniqueIdCount = 0;
      for (var n = 0; n < _BAS_SAFE($NodeList.length)(allNodes); n++) {
        if (_BAS_SAFE($Node.hasAttribute)(allNodes[n], 'id') && _BAS_SAFE($Element.id)(allNodes[n]) === _BAS_SAFE($Element.id)(elm)) uniqueIdCount++;
        if (uniqueIdCount > 1) break;
      }
      if (uniqueIdCount == 1) {
        $unshift(segs, `id("${_BAS_SAFE($Node.getAttribute)(elm, 'id')}")`);
        return $join(segs, '/');
      } else {
        $unshift(segs, $toLowerCase($localName(elm)) + `[@id="${_BAS_SAFE($Node.getAttribute)(elm, 'id')}"]`);
      }
    } else if (_BAS_SAFE($Node.hasAttribute)(elm, 'class')) {
      $unshift(segs, $toLowerCase($localName(elm)) + `[@class="${_BAS_SAFE($Node.getAttribute)(elm, 'class')}"]`);
    } else {
      var sib;
      for (var i = 1, sib = _BAS_SAFE($Node.previousSibling)(elm); sib; sib = _BAS_SAFE($Node.previousSibling)(sib)) {
        if ($localName(sib) === $localName(elm)) i++;
      }
      $unshift(segs, $toLowerCase($localName(elm)) + '[' + i + ']');
    }
  }
  return segs.length ? '/' + $join(segs, '/') : null;
};
