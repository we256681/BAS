window.MonacoEditor = {
  name: 'MonacoEditor',
  props: ['value', 'language'],
  data() {
    return {
      editor: null,
      editorValue: this.value || ''
    }
  },
  mounted() {
    this.initializeEditor();
  },
  watch: {
    value(newValue) {
      if (this.editor && newValue !== this.editor.getValue()) {
        this.editor.setValue(newValue);
      }
    }
  },
  methods: {
    initializeEditor() {
      if (typeof require !== 'undefined') {
        require(['vs/editor/editor.main'], () => {
          this.editor = monaco.editor.create(this.$refs.editor, {
            value: this.editorValue,
            language: this.language || 'javascript',
            theme: 'vs-dark',
            automaticLayout: true,
            minimap: { enabled: false },
            scrollBeyondLastLine: false
          });
          
          this.editor.onDidChangeModelContent(() => {
            const value = this.editor.getValue();
            this.$emit('input', value);
          });
        });
      }
    },
    insertText(text) {
      if (this.editor) {
        const position = this.editor.getPosition();
        this.editor.executeEdits('', [{
          range: new monaco.Range(position.lineNumber, position.column, position.lineNumber, position.column),
          text: text
        }]);
        this.editor.focus();
      }
    },
    getValue() {
      return this.editor ? this.editor.getValue() : this.editorValue;
    },
    setValue(value) {
      if (this.editor) {
        this.editor.setValue(value);
      } else {
        this.editorValue = value;
      }
    }
  },
  template: `
    <div class="monaco-editor-container">
      <div ref="editor" style="height: 400px; border: 1px solid #ddd;"></div>
    </div>
  `
}
