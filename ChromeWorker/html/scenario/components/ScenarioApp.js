window.ScenarioApp = {
  name: 'ScenarioApp',
  components: {
    FunctionManager: window.FunctionManager
  },
  data() {
    return {
      global: {
        threadnumber: 1,
        successnumber: 1,
        failnumber: 1,
        runmode: 'one_time'
      },
      functions: [],
      selectedFunction: null,
      showFunctionModal: false,
      showThreadModal: false,
      showRunModal: false,
      isRecording: false,
      isRunning: false
    }
  },
  computed: {
    runTypeText() {
      if (this.global.successnumber === 1 && this.global.failnumber === 1) {
        return 'One time';
      } else if (this.global.successnumber >= 100000 && this.global.failnumber >= 100000) {
        return 'Repeat';
      } else {
        return 'Custom';
      }
    },
    threadStatusClass() {
      return this.global.threadnumber > 1 ? 'text-success' : 'text-danger';
    },
    runTypeStatusClass() {
      return (this.global.successnumber === 1 && this.global.failnumber === 1) ? 'text-danger' : 'text-success';
    }
  },
  mounted() {
    this.loadScenarioData();
    document.body.style.display = 'block';
  },
  methods: {
    loadScenarioData() {
      this.functions = [
        { id: 1, name: 'Main Function', type: 'main', actions: [] },
        { id: 2, name: 'Login Function', type: 'custom', actions: [] },
        { id: 3, name: 'Data Processing', type: 'custom', actions: [] }
      ];
    },
    selectFunction(func) {
      this.selectedFunction = func;
    },
    editThreadNumber() {
      this.showThreadModal = true;
    },
    editRunMode() {
      this.showRunModal = true;
    },
    updateThreadNumber(newValue) {
      this.global.threadnumber = parseInt(newValue) || 1;
      this.showThreadModal = false;
    },
    updateRunMode(successNumber, failNumber) {
      this.global.successnumber = parseInt(successNumber) || 1;
      this.global.failnumber = parseInt(failNumber) || 1;
      this.showRunModal = false;
    },
    startRecording() {
      this.isRecording = true;
      this.isRunning = false;
    },
    stopRecording() {
      this.isRecording = false;
    },
    runScenario() {
      this.isRunning = true;
      this.isRecording = false;
    },
    stopScenario() {
      this.isRunning = false;
    },
    addFunction(newFunction) {
      if (newFunction) {
        this.functions.push(newFunction);
      } else {
        const func = {
          id: this.functions.length + 1,
          name: `Function ${this.functions.length + 1}`,
          type: 'custom',
          actions: []
        };
        this.functions.push(func);
      }
    },
    deleteFunction(func) {
      const index = this.functions.findIndex(f => f.id === func.id);
      if (index > -1 && func.type !== 'main') {
        this.functions.splice(index, 1);
        if (this.selectedFunction?.id === func.id) {
          this.selectedFunction = null;
        }
      }
    },
    moveAction(payload) {
      const { action, targetFunction } = payload;
      console.log('Moving action', action, 'to function', targetFunction.name);
    }
  },
  template: `
    <div class="scenario-container">
      <div class="container-fluid">
        <!-- Header Controls -->
        <div class="row mb-3">
          <div class="col-md-12">
            <div class="control-panel p-3 border">
              <div class="row">
                <div class="col-md-3">
                  <div class="control-item">
                    <span :class="threadStatusClass">●</span>
                    <span class="small">Thread Number</span>: 
                    <strong>
                      <a href="#" @click.prevent="editThreadNumber()">{{ global.threadnumber }}</a>
                    </strong>
                  </div>
                </div>
                <div class="col-md-3">
                  <div class="control-item">
                    <span :class="runTypeStatusClass">●</span>
                    <span class="small">Run Type</span>: 
                    <strong>
                      <a href="#" @click.prevent="editRunMode()">{{ runTypeText }}</a>
                    </strong>
                  </div>
                </div>
                <div class="col-md-6">
                  <div class="action-buttons">
                    <button 
                      @click="startRecording()"
                      :class="['btn', isRecording ? 'btn-danger' : 'btn-outline-danger']"
                      class="me-2"
                    >
                      {{ isRecording ? 'Stop Recording' : 'Start Recording' }}
                    </button>
                    <button 
                      @click="runScenario()"
                      :class="['btn', isRunning ? 'btn-success' : 'btn-outline-success']"
                      class="me-2"
                    >
                      {{ isRunning ? 'Stop' : 'Run' }}
                    </button>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
        
        <!-- Functions Panel -->
        <div class="row">
          <div class="col-md-4">
            <function-manager 
              :functions="functions"
              @function-selected="selectFunction"
              @function-added="addFunction"
              @function-deleted="deleteFunction"
              @action-moved="moveAction"
            />
          </div>
          <div class="col-md-8">
            <div class="actions-panel">
              <div v-if="selectedFunction" class="function-editor">
                <h5>{{ selectedFunction.name }}</h5>
                <div class="actions-list border p-3" style="min-height: 400px;">
                  <div v-if="selectedFunction.actions.length === 0" class="text-muted text-center">
                    No actions in this function. Start recording or add actions manually.
                  </div>
                  <div 
                    v-for="(action, index) in selectedFunction.actions"
                    :key="index"
                    class="action-item p-2 border mb-1"
                  >
                    {{ action.name }}
                  </div>
                </div>
              </div>
              <div v-else class="text-muted text-center p-5">
                Select a function to edit its actions
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <!-- Thread Number Modal -->
      <div v-if="showThreadModal" class="modal d-block" style="background: rgba(0,0,0,0.5);">
        <div class="modal-dialog">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Edit Thread Number</h5>
              <button @click="showThreadModal = false" class="btn-close"></button>
            </div>
            <div class="modal-body">
              <input 
                type="number" 
                class="form-control" 
                :value="global.threadnumber"
                @keyup.enter="updateThreadNumber($event.target.value)"
                min="1"
                max="100"
              />
            </div>
            <div class="modal-footer">
              <button @click="showThreadModal = false" class="btn btn-secondary">Cancel</button>
              <button @click="updateThreadNumber(document.querySelector('.modal input').value)" class="btn btn-primary">Save</button>
            </div>
          </div>
        </div>
      </div>
      
      <!-- Run Mode Modal -->
      <div v-if="showRunModal" class="modal d-block" style="background: rgba(0,0,0,0.5);">
        <div class="modal-dialog">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Edit Run Mode</h5>
              <button @click="showRunModal = false" class="btn-close"></button>
            </div>
            <div class="modal-body">
              <div class="mb-3">
                <label class="form-label">Success Number:</label>
                <input 
                  type="number" 
                  class="form-control" 
                  :value="global.successnumber"
                  id="successInput"
                />
              </div>
              <div class="mb-3">
                <label class="form-label">Fail Number:</label>
                <input 
                  type="number" 
                  class="form-control" 
                  :value="global.failnumber"
                  id="failInput"
                />
              </div>
            </div>
            <div class="modal-footer">
              <button @click="showRunModal = false" class="btn btn-secondary">Cancel</button>
              <button @click="updateRunMode(document.getElementById('successInput').value, document.getElementById('failInput').value)" class="btn btn-primary">Save</button>
            </div>
          </div>
        </div>
      </div>
    </div>
  `
}
