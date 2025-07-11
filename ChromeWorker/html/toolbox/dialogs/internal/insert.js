BasDialogsLib.insertHelper = {
  insertResource(selector, name, text) {
    this.insert(selector, name, text, 'resource');
  },

  insertVariable(selector, name, text) {
    this.insert(selector, name, text, 'variable');
  },

  insert(selector, name, text, type) {
    const $element = $(selector);

    if ($(`${selector}_number:visible`).length) {
      const $group = $element.closest('.input-group');
      $group.find('.input_selector_number').hide();
      $group.find('.input_selector_string').show();
      $group.find('.selector').html('expression');
      $element.val(text);
    } else {
      if (!$element.is(`[data-${type}-constructor]`)) {
        if ($element.is('[data-is-code-editor]')) {
          const { Editor } = window[$element.attr('id')], position = Editor.getPosition();

          Editor.executeEdits('my-source', [{
            range: monaco.Range.fromPositions(position),
            identifier: { major: 1, minor: 1 },
            forceMoveMarkers: true,
            text: text
          }], [
            monaco.Selection.fromPositions({
              column: position.column + text.length,
              lineNumber: position.lineNumber
            })
          ]);

          Editor.focus();
        } else {
          let start = 0;
          try {
            start = $element.val().length;
            start = cursorPosition($element[0]).start;
          } catch (e) { }
          let val = $element.val();
          val = val.slice(0, start) + text + val.slice(start);
          $element.val(val).trigger('change');
        }
      } else {
        $element.val((_, value) => {
          if (type === 'variable' && $element.is('[data-append-array]')) {
            if (value.length) {
              return [value, name].join(',');
            }
          }

          return name;
        });
      }
    }

    if (selector === '#selector-input') {
      MainView.prototype.pathchanged();
    }

    BasDialogsLib.utils.restoreCursor(selector);
  }
}