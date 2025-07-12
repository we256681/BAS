window.FunctionManager = {
  name: 'FunctionManager',
  props: ['functions'],
  data() {
    return {
      selectedFunction: null,
      showAddModal: false,
      newFunctionName: '',
      draggedAction: null
    }
  },
  computed: {
    sortedFunctions() {
      return this.functions.slice().sort((a, b) => {
        if (a.type === 'main') return -1;
        if (b.type === 'main') return 1;
        return a.name.localeCompare(b.name);
      });
    }
  },
  methods: {
    selectFunction(func) {
      this.selectedFunction = func;
      this.$emit('function-selected', func);
    },
    addFunction() {
      if (this.newFunctionName.trim()) {
        const newFunction = {
          id: Date.now(),
          name: this.newFunctionName.trim(),
          type: 'custom',
          actions: []
        };
        this.$emit('function-added', newFunction);
        this.newFunctionName = '';
        this.showAddModal = false;
      }
    },
    deleteFunction(func) {
      if (func.type !== 'main' && confirm(`Delete function "${func.name}"?`)) {
        this.$emit('function-deleted', func);
      }
    },
    duplicateFunction(func) {
      const duplicated = {
        id: Date.now(),
        name: `${func.name} (Copy)`,
        type: 'custom',
        actions: [...func.actions]
      };
      this.$emit('function-added', duplicated);
    },
    onDragStart(event, action) {
      this.draggedAction = action;
      event.dataTransfer.effectAllowed = 'move';
    },
    onDragOver(event) {
      event.preventDefault();
      event.dataTransfer.dropEffect = 'move';
    },
    onDrop(event, targetFunction) {
      event.preventDefault();
      if (this.draggedAction && targetFunction) {
        this.$emit('action-moved', {
          action: this.draggedAction,
          targetFunction: targetFunction
        });
      }
      this.draggedAction = null;
    }
  },
  template: `
    <div class="function-manager">
      <div class="function-header d-flex justify-content-between align-items-center mb-3">
        <h5>Functions</h5>
        <button @click="showAddModal = true" class="btn btn-sm btn-primary">
          <i class="fa fa-plus"></i> Add Function
        </button>
      </div>
      
      <div class="functions-list">
        <div 
          v-for="func in sortedFunctions"
          :key="func.id"
          @click="selectFunction(func)"
          @dragover="onDragOver"
          @drop="onDrop($event, func)"
          :class="['function-item', 'p-3', 'border', 'mb-2', selectedFunction?.id === func.id ? 'border-primary bg-light' : 'border-light']"
          style="cursor: pointer;"
        >
          <div class="d-flex justify-content-between align-items-center">
            <div class="function-info">
              <div class="function-name">
                <strong>{{ func.name }}</strong>
                <span v-if="func.type === 'main'" class="badge bg-primary ms-2">Main</span>
              </div>
              <div class="text-muted small">
                {{ func.actions.length }} action{{ func.actions.length !== 1 ? 's' : '' }}
              </div>
            </div>
            <div v-if="func.type !== 'main'" class="function-actions">
              <button 
                @click.stop="duplicateFunction(func)"
                class="btn btn-sm btn-outline-secondary me-1"
                title="Duplicate"
              >
                <i class="fa fa-copy"></i>
              </button>
              <button 
                @click.stop="deleteFunction(func)"
                class="btn btn-sm btn-outline-danger"
                title="Delete"
              >
                <i class="fa fa-trash"></i>
              </button>
            </div>
          </div>
          
          <div v-if="func.actions.length > 0" class="actions-preview mt-2">
            <div class="small text-muted">Actions:</div>
            <div 
              v-for="(action, index) in func.actions.slice(0, 3)"
              :key="index"
              @dragstart="onDragStart($event, action)"
              draggable="true"
              class="action-preview small p-1 border-start border-2 border-secondary ms-2"
            >
              {{ action.name || action.type }}
            </div>
            <div v-if="func.actions.length > 3" class="small text-muted ms-2">
              ... and {{ func.actions.length - 3 }} more
            </div>
          </div>
        </div>
      </div>
      
      <!-- Add Function Modal -->
      <div v-if="showAddModal" class="modal d-block" style="background: rgba(0,0,0,0.5);">
        <div class="modal-dialog">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Add New Function</h5>
              <button @click="showAddModal = false" class="btn-close"></button>
            </div>
            <div class="modal-body">
              <div class="mb-3">
                <label class="form-label">Function Name:</label>
                <input 
                  v-model="newFunctionName"
                  @keyup.enter="addFunction()"
                  type="text" 
                  class="form-control" 
                  placeholder="Enter function name..."
                  autofocus
                />
              </div>
            </div>
            <div class="modal-footer">
              <button @click="showAddModal = false" class="btn btn-secondary">Cancel</button>
              <button @click="addFunction()" :disabled="!newFunctionName.trim()" class="btn btn-primary">Add Function</button>
            </div>
          </div>
        </div>
      </div>
    </div>
  `
}
