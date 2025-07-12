window.ToolboxApp = {
  name: 'ToolboxApp',
  components: {
    MonacoEditor: window.MonacoEditor
  },
  data() {
    return {
      currentAction: null,
      codeEditor: null,
      codeValue: '',
      searchQuery: '',
      showVariableModal: false,
      showResourceModal: false,
      actions: [],
      filteredActions: [],
      selectedCategory: 'all'
    }
  },
  computed: {
    displayActions() {
      if (!this.searchQuery) return this.filteredActions;
      return this.filteredActions.filter(action => 
        action.name.toLowerCase().includes(this.searchQuery.toLowerCase()) ||
        action.description.toLowerCase().includes(this.searchQuery.toLowerCase())
      );
    }
  },
  mounted() {
    this.initializeMonacoEditor();
    this.loadActions();
    document.body.style.display = 'block';
  },
  methods: {
    initializeMonacoEditor() {
      if (typeof require !== 'undefined') {
        require(['vs/editor/editor.main'], () => {
          const editorElement = document.getElementById('Code');
          if (editorElement && typeof createCodeEditor === 'function') {
            this.codeEditor = createCodeEditor(editorElement, {
              value: window.Code ? window.Code.InitialValue : ''
            });
          }
        });
      }
    },
    loadActions() {
      this.actions = [
        { name: 'Custom Code', description: 'Execute custom JavaScript code', category: 'code' },
        { name: 'Click', description: 'Click on element', category: 'browser' },
        { name: 'Type', description: 'Type text into element', category: 'browser' },
        { name: 'Wait', description: 'Wait for specified time', category: 'flow' },
        { name: 'If', description: 'Conditional execution', category: 'flow' },
        { name: 'Loop', description: 'Repeat actions', category: 'flow' }
      ];
      this.filteredActions = this.actions;
    },
    selectAction(action) {
      this.currentAction = action;
      this.$emit('action-selected', action);
    },
    filterByCategory(category) {
      this.selectedCategory = category;
      if (category === 'all') {
        this.filteredActions = this.actions;
      } else {
        this.filteredActions = this.actions.filter(action => action.category === category);
      }
    },
    showVariableInserter() {
      this.showVariableModal = true;
    },
    showResourceInserter() {
      this.showResourceModal = true;
    },
    insertVariable(variable) {
      if (this.$refs.editor) {
        this.$refs.editor.insertText(`[[${variable}]]`);
      }
      this.showVariableModal = false;
    },
    insertResource(resource) {
      if (this.$refs.editor) {
        this.$refs.editor.insertText(`{{${resource}}}`);
      }
      this.showResourceModal = false;
    }
  },
  template: `
    <div class="toolbox-container">
      <div class="container-fluid">
        <div class="row">
          <div class="col-md-3">
            <div class="search-panel">
              <input 
                v-model="searchQuery" 
                type="text" 
                class="form-control" 
                placeholder="Search actions..."
              />
              <div class="category-filters mt-2">
                <button 
                  v-for="category in ['all', 'browser', 'code', 'flow']"
                  :key="category"
                  @click="filterByCategory(category)"
                  :class="['btn', 'btn-sm', selectedCategory === category ? 'btn-primary' : 'btn-outline-primary']"
                  class="me-1 mb-1"
                >
                  {{ category }}
                </button>
              </div>
            </div>
            <div class="actions-list mt-3">
              <div 
                v-for="action in displayActions"
                :key="action.name"
                @click="selectAction(action)"
                :class="['action-item', 'p-2', 'border', 'mb-1', currentAction?.name === action.name ? 'border-primary bg-light' : 'border-light']"
                style="cursor: pointer;"
              >
                <strong>{{ action.name }}</strong>
                <div class="text-muted small">{{ action.description }}</div>
              </div>
            </div>
          </div>
          <div class="col-md-9">
            <div class="editor-panel">
              <monaco-editor 
                ref="editor"
                v-model="codeValue"
                language="javascript"
              />
              <div class="editor-controls mt-2 text-center">
                <button 
                  @click="showVariableInserter()"
                  class="btn btn-outline-primary me-2"
                >
                  <i class="fa fa-at"></i> Insert Variable
                </button>
                <button 
                  @click="showResourceInserter()"
                  class="btn btn-outline-primary"
                >
                  <i class="fa fa-database"></i> Load from Resource
                </button>
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <!-- Variable Modal -->
      <div v-if="showVariableModal" class="modal d-block" style="background: rgba(0,0,0,0.5);">
        <div class="modal-dialog">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Insert Variable</h5>
              <button @click="showVariableModal = false" class="btn-close"></button>
            </div>
            <div class="modal-body">
              <div class="variable-list">
                <div 
                  v-for="variable in ['ELEMENT_TEXT', 'ELEMENT_HTML', 'CURRENT_URL', 'RANDOM_STRING']"
                  :key="variable"
                  @click="insertVariable(variable)"
                  class="variable-item p-2 border mb-1"
                  style="cursor: pointer;"
                >
                  {{ variable }}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <!-- Resource Modal -->
      <div v-if="showResourceModal" class="modal d-block" style="background: rgba(0,0,0,0.5);">
        <div class="modal-dialog">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Load from Resource</h5>
              <button @click="showResourceModal = false" class="btn-close"></button>
            </div>
            <div class="modal-body">
              <div class="resource-list">
                <div 
                  v-for="resource in ['PROXIES', 'ACCOUNTS', 'USER_AGENTS', 'EMAILS']"
                  :key="resource"
                  @click="insertResource(resource)"
                  class="resource-item p-2 border mb-1"
                  style="cursor: pointer;"
                >
                  {{ resource }}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  `
}
