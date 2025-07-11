

window.Modals = {

    props: [],
    data: function()
    {
      return {
      }
    },
    
    methods: {
      ShowError: function(){
        
      }
    },

    updated: function()
    {
      
    },
    

    template: `<div>
          <div id="EditDurationModal" uk-modal="stack: true">
            <div class="uk-modal-dialog uk-modal-body">
              <h2 class="uk-modal-title" id="EditDurationModalName"></h2>
              <p>
                <form class="uk-form-horizontal uk-margin-large">

                    <div class="uk-margin">
                        <label class="uk-form-label" for="EditDurationDays">{{$t('Days :')}} </label>
                        <div class="uk-form-controls">
                            <input class="uk-input" id="EditDurationDays" type="number" :placeholder="$t('Days')" min="0" style="width: calc(100% - 30px);">
                            <a href="#" @click.prevent="$('#EditDurationDays').val(0)" style="margin-left: 5px;" :uk-tooltip="$t('Set to zero')">
                              <span uk-icon="icon: refresh"></span>
                            </a>
                        </div>
                    </div>

                    <div class="uk-margin">
                        <label class="uk-form-label" for="EditDurationHours">{{$t('Hours :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="23" class="uk-input" id="EditDurationHours" style="width: calc(100% - 30px);">
                                
                            <a href="#" @click.prevent="$('#EditDurationHours').val(0)" style="margin-left: 5px;" :uk-tooltip="$t('Set to zero')">
                              <span uk-icon="icon: refresh"></span>
                            </a>
                        </div>
                    </div>

                    <div class="uk-margin">
                        <label class="uk-form-label" for="EditDurationMinutes">{{$t('Minutes :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="59" class="uk-input" id="EditDurationMinutes" style="width: calc(100% - 30px);">
                                
                            <a href="#" @click.prevent="$('#EditDurationMinutes').val(0)" style="margin-left: 5px;" :uk-tooltip="$t('Set to zero')">
                              <span uk-icon="icon: refresh"></span>
                            </a>

                        </div>
                    </div>

                    <div class="uk-margin">
                        <label class="uk-form-label"></label>
                        <div class="uk-form-controls" style="text-align:right">
                            <a href="#" id="EditDurationInfinite">{{$t('Set to inifinite')}}</a>
                        </div>
                    </div>

                </form>

              </p>
              <p class="uk-text-right">
                  <button class="uk-button uk-button-default uk-modal-close" type="button">{{$t('Cancel')}}</button>
                  <button class="uk-button uk-button-primary" type="button" id="EditDurationModalSave">{{$t('Save')}}</button>
              </p>

            </div>
          </div>

          <div id="EditSchedulerSectionModal" uk-modal="stack: true">
            <div class="uk-modal-dialog uk-modal-body">
              <h2 class="uk-modal-title" id="EditSchedulerSectionModalName"></h2>
              <p>
                 <form class="uk-form-horizontal uk-margin-large">

                    <div class="uk-margin" id="EditSchedulerSectionDaysPanel">
                        <label class="uk-form-label" for="EditSchedulerSectionDays">{{$t('Day of the month :')}} </label>
                        <div class="uk-form-controls">
                            <input class="uk-input" id="EditSchedulerSectionDays" type="number" :placeholder="$t('Day of the month')" min="1" max="31">
                        </div>
                    </div>
                    <div class="uk-margin" id="EditSchedulerSectionWeeksPanel">
                        <label class="uk-form-label" for="EditSchedulerSectionWeeks">{{$t('Day of the week :')}} </label>
                        <div class="uk-form-controls">
                            <select class="uk-select" id="EditSchedulerSectionWeeks" :placeholder="$t('Day of the week')">
                                <option value="1">{{$t('Monday')}}</option>
                                <option value="2">{{$t('Tuesday')}}</option>
                                <option value="3">{{$t('Wednesday')}}</option>
                                <option value="4">{{$t('Thursday')}}</option>
                                <option value="5">{{$t('Friday')}}</option>
                                <option value="6">{{$t('Saturday')}}</option>
                                <option value="7">{{$t('Sunday')}}</option>
                            </select>
                        </div>
                    </div>
                    <div class="uk-margin" id="EditSchedulerSectionHoursPanel">
                        <label class="uk-form-label" for="EditSchedulerSectionHours">{{$t('Hour :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="23" class="uk-input" id="EditSchedulerSectionHours">

                        </div>
                    </div>

                    <div class="uk-margin" id="EditSchedulerSectionMinutesPanel">
                        <label class="uk-form-label" for="EditSchedulerSectionMinutes">{{$t('Minute :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="59" class="uk-input" id="EditSchedulerSectionMinutes">
                        </div>
                    </div>

                    <a href="#" id="EditSchedulerSectionProbability" :uk-tooltip="$t('Probability settings allows to start task at certain task with selected probability. It also allows to specify interval, when task will start instead of exact time.')">{{$t('Run with probability')}}</a>

                    <div class="uk-margin" id="EditSchedulerSectionDaysPanelTo">
                        <label class="uk-form-label" for="EditSchedulerSectionDaysTo">{{$t('To day of the month :')}} </label>
                        <div class="uk-form-controls">
                            <input class="uk-input" id="EditSchedulerSectionDaysTo" type="number" :placeholder="$t('Day of the month')" min="1" max="31">
                        </div>
                    </div>
                    <div class="uk-margin" id="EditSchedulerSectionWeeksPanelTo">
                        <label class="uk-form-label" for="EditSchedulerSectionWeeksTo">{{$t('To day of the week :')}} </label>
                        <div class="uk-form-controls">
                            <select class="uk-select" id="EditSchedulerSectionWeeksTo" :placeholder="$t('Day of the week')">
                                <option value="1">{{$t('Monday')}}</option>
                                <option value="2">{{$t('Tuesday')}}</option>
                                <option value="3">{{$t('Wednesday')}}</option>
                                <option value="4">{{$t('Thursday')}}</option>
                                <option value="5">{{$t('Friday')}}</option>
                                <option value="6">{{$t('Saturday')}}</option>
                                <option value="7">{{$t('Sunday')}}</option>
                            </select>
                        </div>
                    </div>
                    <div class="uk-margin" id="EditSchedulerSectionHoursPanelTo">
                        <label class="uk-form-label" for="EditSchedulerSectionHoursTo">{{$t('To hour :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="23" class="uk-input" id="EditSchedulerSectionHoursTo">

                        </div>
                    </div>

                    <div class="uk-margin" id="EditSchedulerSectionMinutesPanelTo">
                        <label class="uk-form-label" for="EditSchedulerSectionMinutesTo">{{$t('To minute :')}} </label>
                        <div class="uk-form-controls">
                            <input type="number" min="0" max="59" class="uk-input" id="EditSchedulerSectionMinutesTo">
                        </div>
                    </div>

                    <div class="uk-margin" id="EditSchedulerSectionProbabilityDataPanel">
                        <label class="uk-form-label" for="EditSchedulerSectionProbabilityData">{{$t('Run probability for current period :')}} </label>
                        <div class="uk-form-controls">
                            <div style="text-align: center" id="EditSchedulerSectionProbabilityLabel">{{$t('100%')}}</div>
                            <input class="uk-range" type="range" value="100" min="1" max="100" step="1" id="EditSchedulerSectionProbabilityData">

                            
                        </div>
                    </div>

                    <div class="uk-margin" id="EditSchedulerSectionValidation" style="text-align:right">
                        {{$t('"From" time is greater than "to" time.')}}
                    </div>


                </form>
              </p>
              <p class="uk-text-right">
                  <button class="uk-button uk-button-default uk-modal-close" type="button">{{$t('Cancel')}}</button>
                  <button class="uk-button uk-button-primary" type="button" id="EditSchedulerSectionModalSave">{{$t('Save')}}</button>
              </p>
            </div>
          </div>

          <div id="EditDateModal" uk-modal="stack: true">
            <div class="uk-modal-dialog uk-modal-body">
              <h2 class="uk-modal-title" id="EditDateModalName"></h2>
              <p>
                <div class="datepicker-here" data-timepicker="true" data-language='en' data-time-format='hh:ii'></div>
              </p>
              <p class="uk-text-right">
                  <button class="uk-button uk-button-default uk-modal-close" type="button">{{$t('Cancel')}}</button>
                  <button class="uk-button uk-button-primary" type="button" id="EditDateModalSave">{{$t('Save')}}</button>
              </p>
            </div>
          </div>

          <div id="EditNumberModal" uk-modal="stack: true">
            <div class="uk-modal-dialog uk-modal-body">
              <h2 class="uk-modal-title" id="EditNumberModalName"></h2>
              <p>
                <input type="number" min="1" class="uk-input" id="EditNumberModalData" />
              </p>
              <p class="uk-text-right">
                  <button class="uk-button uk-button-default uk-modal-close" type="button">{{$t('Cancel')}}</button>
                  <button class="uk-button uk-button-primary" type="button" id="EditNumberModalSave">{{$t('Save')}}</button>
              </p>
            </div>
          </div>

          <div id="EditTaskSequenceModal" uk-modal="stack: true">
            <div class="uk-modal-dialog uk-modal-body">
              <h2 class="uk-modal-title" id="EditTaskSequenceModalName"></h2>
              <p>
                <select class="uk-select" id="EditTaskSequenceModalData" :placeholder="$t('Task name')">
                </select>
                <div id="EditTaskSequenceNoData">
                    {{$t('Task list is empty. Please create one.')}}
                </div>
              </p>
              <p class="uk-text-right">
                  <button class="uk-button uk-button-default uk-modal-close" type="button">{{$t('Cancel')}}</button>
                  <button class="uk-button uk-button-primary" type="button" id="EditTaskSequenceModalSave">{{$t('Save')}}</button>
              </p>
            </div>
          </div>


        </div>`

}