(function (window) {
  const ActionUpdaterModel = Backbone.Model.extend({
    defaults: {
      isStarted: false,
      successCount: 0,
      errorsCount: 0,
      tasks: []
    },

    handle(id, message, error = true) {
      if (!error) {
        this.set('successCount', this.get('successCount') + 1);
      } else {
        this.set('errorsCount', this.get('errorsCount') + 1);
      }
      this.trigger('log', { message, id });
    },

    isUnsuccessfulUpdate() {
      if (this.get('isStarted')) return false;
      const success = this.get('successCount');
      return success !== _.size(this.get('tasks'));
    },

    isSuccessfulUpdate() {
      if (this.get('isStarted')) return false;
      const success = this.get('successCount');
      return success === _.size(this.get('tasks'));
    },

    waitForBackup() {
      return new Promise((resolve) => {
        this.once('backup', (path) => resolve());
        BrowserAutomationStudio_StartBackup();
      });
    },

    initialize() {
      this.on('change:isStarted', async (__, isStarted) => {
        if (!isStarted) {
          BrowserAutomationStudio_TriggerEvent('scenario.updateFinish');
        } else {
          BrowserAutomationStudio_TriggerEvent('scenario.updateStart');
        }
        if (isStarted) await _.sleep(200).then(() => this.run());
      });

      this.on('finish', () => {
        if (!this.get('errorsCount')) this.trigger('log', {
          message: [
            `${tr(`Done without errors`)}.`,
            `${tr('Completed:')} ${this.get('successCount')}.`,
            `${tr('Errors:')} ${this.get('errorsCount')}.`
          ].join(' '),
          color: '#669fc2'
        });

        if (this.get('errorsCount')) this.trigger('log', {
          message: [
            `${tr(`Done with errors`)}.`,
            `${tr('Completed:')} ${this.get('successCount')}.`,
            `${tr('Errors:')} ${this.get('errorsCount')}.`
          ].join(' '),
          color: '#d8695f'
        });
      });
    },

    async run() {
      await this.waitForBackup({});
      this.set('successCount', 0);
      this.set('errorsCount', 0);
      let timeout = undefined;

      for (const { id, dat, index, isDatEmpty, isDatDamaged } of this.get('tasks')) {
        const { error, message } = await new Promise((resolve) => {
          _MainView.currentTargetId = index;

          if (dat && !_A[dat['s']]) {
            resolve({ error: true, message: tr('The module containing this action is damaged or disabled.') });
          } else if (isDatDamaged) {
            resolve({ error: true, message: tr('The technical description of this action is damaged.') });
          } else if (isDatEmpty) {
            resolve({ error: true, message: tr('The technical description of this action is empty.') });
          } else {
            timeout = setTimeout(() => resolve({ error: true, message: tr('Timeout during the action update.') }), 10000);

            this.off('toolbox.editStarted').once('toolbox.editStarted', () => {
              this.off('toolbox.editSuccess').once('toolbox.editSuccess', (data) => {
                this.off('toolbox.editEnd').once('toolbox.editEnd', () => {
                  resolve({ error: false, message: data });
                });
                BrowserAutomationStudio_EditEnd();
              });

              this.off('toolbox.editFail').once('toolbox.editFail', (data) => {
                this.off('toolbox.editEnd').once('toolbox.editEnd', () => {
                  resolve({ error: true, message: data });
                });
                BrowserAutomationStudio_EditEnd();
              });

              BrowserAutomationStudio_EditSaveStart();
            });

            _MainView.Edit({ disableModal: true });
          }
        }).finally(() => clearTimeout(timeout));

        this.handle(id, message, !!error);
        if (!this.get('isStarted')) break;
      }

      this.set('isStarted', false).trigger('finish');
    }
  });

  const ActionUpdaterModal = Backbone.View.extend({
    template: _.template(/*html*/`
      <div class="modal-dialog" role="document">
        <div class="action-updater-modal-content">
          <div class="action-updater-modal-header">
            <h4 class=""><%= tr('The BAS version of the project is different from the current BAS version, update the project actions?') %></h4>
          </div>
          <div class="action-updater-modal-body">
            <div class="action-updater-modal-text"><%= tr('The same actions may differ in different BAS versions. To update an action to the current BAS version, you need to use this tool or recreate each action manually.') %></div>
            <div class="action-updater-modal-text"><%= tr('Actions that are not updated may not work correctly or even cause an error.') %></div>
            <div class="action-updater-modal-text"><%= tr('This tool allows you to automatically update all actions in the project.') %></div>
            <div class="action-updater-modal-text"><%= tr('You can always open it in the ') %><a class="action-updater-modal-link" id="actionUpdaterModalMenu"><%= tr('context menu') %></a>.</div>
          </div>
          <div class="action-updater-modal-footer">
            <button type="button" id="actionUpdaterModalAccept" class="btn-base btn-accept" data-dismiss="modal"><%= tr('OK') %></button>
            <button type="button" id="actionUpdaterModalCancel" class="btn-base btn-cancel" data-dismiss="modal"><%= tr('Cancel') %></button>
          </div>
        </div>
      </div>
    `),

    id: 'actionUpdaterModal',

    className: 'modal',

    tagName: 'div',

    events: {
      'click #actionUpdaterModalAccept': function () {
        this.trigger('accept');
        this.hide();
      },
      'click #actionUpdaterModalCancel': function () {
        this.trigger('cancel');
        this.hide();
      },
      'click #actionUpdaterModalMenu': function () {
        BrowserAutomationStudio_HighlightMenuItem("ShowUpdater");
      }
    },

    initialize() {
      _MainView.once('render', () => {
        if (_TaskCollection.length === 1) return;

        function compareVersion(v1, v2) {
          v1 = v1.split('.');
          v2 = v2.split('.');
          for (let i = 0; i < Math.min(v1.length, v2.length); ++i) {
            v1[i] = parseInt(v1[i], 10);
            v2[i] = parseInt(v2[i], 10);
            if (v1[i] > v2[i]) return 1;
            if (v1[i] < v2[i]) return -1;
          }
          return v1.length === v2.length ? 0 : (v1.length < v2.length ? -1 : 1);
        }

        if (compareVersion(_ApplicationEngineVersion, _ScriptEngineVersion) !== 0) this.show();
      });
    },

    render() {
      if (!this.$el.is(':empty')) return this;
      this.$el.html(this.template()).modal({
        backdrop: 'static',
        keyboard: false,
        show: false,
      });
      return this;
    },

    show() {
      if (this.$el.is(':visible')) return this;
      this.render().$el.modal('show');
      return this;
    },

    hide() {
      if (this.$el.is(':hidden')) return this;
      this.render().$el.modal('hide');
      return this;
    }
  });

  const ActionUpdaterView = Backbone.View.extend({
    template: _.template(/*html*/`
      <div class="action-updater-header">
        <h1><%= tr('Auto-update actions') %></h1>
        <h2><%= tr('This tool allows you to automatically update actions in the project.') %></h2>
      </div>
      <div class="action-updater-panel">
        <div id="actionUpdaterProgress" class="action-updater-progress"></div>
        <select id="actionUpdaterSelect">
          <option class="action-updater-select-option" value="all" selected="selected"><%= tr('All actions in the project') %></option>
          <option class="action-updater-select-option" value="current"><%= tr('All actions in the current function') %></option>
          <option class="action-updater-select-option" value="selected"><%= tr('Only the selected actions') %></option>
        </select>
        <div id="actionUpdaterLog" class="action-updater-log"></div>
        <div class="action-updater-stats">
          <span class="action-updater-stats-item">
            <svg width="14" height="14" viewBox="0 0 14 14" fill="none">
              <path fill-opacity="0.5" fill-rule="evenodd" clip-rule="evenodd" d="M0 7C0 3.13401 3.13401 0 7 0C10.866 0 14 3.13401 14 7C14 10.866 10.866 14 7 14C3.13401 14 0 10.866 0 7ZM7.5215 4.9525L7.2275 8.897H6.48025L6.223 4.9525V2.625H7.5215V4.9525ZM7.60725 9.88925V11.347H6.125V9.88925H7.60725Z" fill="white" />
            </svg>          
            <span><%= tr('Total actions:') %> <span id="actionUpdaterTotalCount">0</span></span>
          </span>
          <span class="action-updater-stats-item">
            <svg width="14" height="14" viewBox="0 0 14 14" fill="none">
              <path d="M7 0C3.13473 0 0 3.13473 0 7C0 10.8653 3.13473 14 7 14C10.8653 14 14 10.8653 14 7C14 3.13473 10.8653 0 7 0ZM6.04545 9.86364L3.18182 7L4.13636 6.04545L6.04545 7.95455L9.83564 4.16436L10.7902 5.06355L6.04545 9.86364Z" fill="#669FC2"/>
            </svg>          
            <span><%= tr('Completed:') %> <span id="actionUpdaterSuccessCount">0</span></span>
          </span>
          <span class="action-updater-stats-item">
            <svg width="14" height="14" viewBox="0 0 14 14" fill="none">
              <path fill-rule="evenodd" clip-rule="evenodd" d="M2.05004 2.05004C4.78365 -0.683571 9.21637 -0.683121 11.9495 2.05004C13.3148 3.41533 13.9982 5.2047 13.9996 6.99454C14.0008 8.56382 13.4776 10.1334 12.43 11.4179C12.2806 11.6012 12.1204 11.7787 11.9495 11.9495C9.21592 14.6831 4.7832 14.6827 2.05004 11.9495C-0.683572 9.21592 -0.683121 4.7832 2.05004 2.05004ZM6.99979 7.89974L8.79969 9.69965L9.69965 8.79969L7.89974 6.99979L9.69965 5.19988L8.7997 4.29992L6.99979 6.09983L5.19988 4.29992L4.29992 5.19988L6.09983 6.99979L4.29992 8.79969L5.19988 9.69965L6.99979 7.89974Z" fill="#D8695F"/>
            </svg>          
            <span><%= tr('Errors:') %> <span id="actionUpdaterErrorsCount">0</span></span>
          </span>
        </div>
        <button id="actionUpdaterCopyLog" class="action-updater-copy-btn" style="display: none">
          <svg width="13" height="15" viewBox="0 0 13 15" fill="none">
            <path d="M10 3V0H0V12H4V15H13V3H10ZM4 11H1V1H9V3H4V11ZM12 14H5V4H12V14Z" fill="white"/>
          </svg>
          <span style="margin-left: 13px"><%= tr('Copy log to clipboard') %></span>
        </button>
        <button id="actionUpdaterCopyBackup" class="action-updater-copy-btn" style="display: none">
          <span><%= tr('Copy the path to a project backup') %> (<span id="actionUpdaterBackupName"></span>)</span>
        </button>
      </div>
      <div class="action-updater-footer">
        <button type="button" id="actionUpdaterAccept" class="btn-base btn-accept"><%= tr('Run') %></button>
        <button type="button" id="actionUpdaterCancel" class="btn-base btn-cancel"><%= tr('Close') %></button>
      </div>
    `),

    className: 'action-updater',

    initialize() {
      this.model = new ActionUpdaterModel();
      this.modal = new ActionUpdaterModal();

      this.model.on('change:isStarted', (__, isStarted) => {
        if (isStarted) this.$('#actionUpdaterProgress').progressBar('reset');

        if (this.model.isSuccessfulUpdate() && !isStarted) {
          this.$('#actionUpdaterAccept').prop('disabled', !isStarted);
        } else {
          this.$('#actionUpdaterAccept').prop('disabled', isStarted);
        }

        this.$('#actionUpdaterSelect').prop('disabled', isStarted);
        this.$('#actionUpdaterSelect').selectpicker('refresh');
        if (isStarted) this.$('#actionUpdaterLog').empty();
      });

      this.model.on('change:successCount', (__, count) => {
        this.$('#actionUpdaterProgress').progressBar('step');
        this.$('#actionUpdaterSuccessCount').text(count);
      });

      this.model.on('change:errorsCount', (__, count) => {
        this.$('#actionUpdaterProgress').progressBar('step');
        this.$('#actionUpdaterErrorsCount').text(count);
      });

      this.model.on('change:tasks', (__, { length }) => {
        this.$('#actionUpdaterAccept').prop('disabled', length === 0);
        this.$('#actionUpdaterSelect').prop('disabled', false);
        this.$('#actionUpdaterSelect').selectpicker('refresh');
        this.$('#actionUpdaterProgress').data('max', length);
        this.$('#actionUpdaterProgress').progressBar('reset');

        this.$('#actionUpdaterTotalCount').text(length);
        this.$('#actionUpdaterSuccessCount').text(0);
        this.$('#actionUpdaterErrorsCount').text(0);
      });

      this.model.on('backup', (path) => {
        this.$('#actionUpdaterBackupName').text(path.slice(1 + Math.max(
          path.lastIndexOf('\\'),
          path.lastIndexOf('\/'),
        )));
        this.$('#actionUpdaterCopyBackup').data('path', path);
        this.$('#actionUpdaterCopyBackup').slideDownEx(250);
      });

      this.modal.on('accept', this.show, this);
      this.modal.on('cancel', this.hide, this);
      this.model.on('log', this.log, this);
      this.on('show', this.update, this);
    },

    log({ message, color, id }) {
      if (message) {
        const identifier = id ? `[${id}]:` : '';

        $('<div>', { class: 'action-updater-log-message' })
          .append($('<span>', { text: identifier })
            .css('color', color))
          .append($('<span>', { text: message })
            .css('color', color))
          .appendTo(this.$('#actionUpdaterLog'))
      }
    },

    render() {
      if (!this.$el.is(':empty')) return this;
      this.$el.html(this.template()).appendTo('body');
      this.$('#actionUpdaterProgress').progressBar();
      this.$('#actionUpdaterSelect').selectpicker({
        style: 'action-updater-select',
        template: { caret: '' },
        container: false,
        width: false
      });
      return this;
    },

    update() {
      const type = this.$('#actionUpdaterSelect').val();
      this.$('.action-updater-select').trigger('blur');
      const tasks = window.App.utils.filterTasks(type);
      this.model.unset('tasks', { silent: true });
      this.model.set('tasks', tasks);
    },

    show() {
      if (!this.$el.is(':visible') && !this.model.get('isStarted')) {
        $('body,html').toggleClass('overflow-hidden');
        this.render().$el.show();
        this.$('#actionUpdaterProgress').hide();
        this.trigger('show');
      }
      this.modal.hide();
      return this;
    },

    hide() {
      if (!this.$el.is(':hidden') && !this.model.get('isStarted')) {
        $('body,html').toggleClass('overflow-hidden');
        this.render().$el.hide();
        this.$('#actionUpdaterProgress').hide();
        this.trigger('hide');
      }
      this.modal.hide();
      return this;
    },

    events: {
      'click #actionUpdaterCopyBackup': function () {
        if (!window.getSelection().toString().length) {
          const $button = this.$('#actionUpdaterCopyBackup');
          const $input = $('<textarea>').appendTo('body');
          $input.val($button.data('path')).select();
          document.execCommand('copy');
          $input.remove();
        }
      },

      'click #actionUpdaterCopyLog': function () {
        if (!window.getSelection().toString().length) {
          const data = $.map(this.$('#actionUpdaterLog div'), $.text);
          const $input = $('<textarea>').appendTo('body');
          $input.val(data.join('\r\n')).select();
          document.execCommand('copy');
          $input.remove();
        }
      },

      'click #actionUpdaterCancel': function () {
        if (!this.model.get('isStarted')) return this.hide();
        this.$('#actionUpdaterProgress').slideDownEx(250);
        this.$('#actionUpdaterCopyLog').slideDownEx(250);
        this.model.set('isStarted', false);
      },

      'click #actionUpdaterAccept': function () {
        if (this.model.get('isStarted')) return this.hide();
        this.$('#actionUpdaterProgress').slideDownEx(250);
        this.$('#actionUpdaterCopyLog').slideDownEx(250);
        this.model.set('isStarted', true);
      },

      'change #actionUpdaterSelect': 'update',
    }
  });

  window.App.ActionUpdater = ActionUpdaterView;
})(window);