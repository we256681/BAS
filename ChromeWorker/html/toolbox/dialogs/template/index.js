BasDialogsLib.templates = {
  main: _.template(`
    <div id="modalDialogContainer">
      <div id="modalSearchContainer">
        <input type="text" id="modalSearchInput" placeholder="<%= tr('Start typing ' + metadata.singleName + ' name...') %>">
        <span id="modalSearchIcon" class="modal-search-control">
          <svg width="34" height="34" viewBox="0 0 34 34" fill="none" xmlns="http://www.w3.org/2000/svg">
            <path d="M16.167 22.833a6.667 6.667 0 100-13.333 6.667 6.667 0 000 13.333zM24.5 24.5l-3.625-3.625" stroke="#fff" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" />
          </svg>        
        </span>
        <button id="modalSearchClose" class="modal-search-control">
          <svg width="20" height="20" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
            <path d="M14 6l-8 8M6 6l8 8" stroke="#c56d5f" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
          </svg>
        </button>
      </div>
      <div id="modalContentContainer">
        <div id="modalListContainer">
          <div id="modalListWrapper">
            <div id="modalListContent">
              <ul id="modalListItems">
                <% _.keys(map).forEach((header) => { %>
                  <% map[header].forEach((item, index) => { %>
                    <li class="modal-list-item" data-index="<%= index %>" data-id="<%= item.id %>">
                      <div class="modal-list-wrap">
                        <div class="modal-list-header <%= index === 0 ? 'modal-header-visible' : 'modal-header-hidden' %>">
                          <div class="modal-list-header-text"><%= header %></div>
                        </div>
                        <div class="modal-list-content" data-id="<%= item.id %>">
                          <div class="modal-list-text-lg modal-text-nowrap modal-text-<%= metadata.color %>" data-title="<%= item.name %>">
                            <%= metadata.template({ ...item, isRecent: false }) %>
                          </div>
                          <% if (item.description) { %>
                            <div class="modal-list-item-desc" 
                              data-clickable="<%= item.ref ? true : false %>"
                              data-description="<%= item.description %>"
                              data-popup="<%= item.popup %>"
                              data-ref="<%= item.ref %>">
                              <svg width="9" height="3" viewBox="0 0 9 3" fill="none" xmlns="http://www.w3.org/2000/svg">
                                <rect x="0.5" y="2.5" width="2" height="2" transform="rotate(-90 0.5 2.5)" fill="white" stroke="#bdbdbd" />
                                <rect y="2" width="1" height="9" transform="rotate(-90 0 2)" fill="#bdbdbd" />
                              </svg>    
                              <div class="modal-list-text-sm modal-text-nowrap" data-title="<%= tr(item.description) %>">
                                <%= tr(item.description) %>
                              </div>
                            </div>
                          <% } %>
                        </div>
                      </div>
                    </li>
                  <% }); %>
                <% }); %>
              </ul>
            </div>
          </div>
          <div id="modalListEmpty">
            <% if (!items.length) { %>
              <%= tr('No ' + metadata.singleName + ' has been created') %>
            <% } else { %>
              <%= tr('No ' + metadata.pluralName + ' found') %>
            <% } %>
          </div>
          <div id="modalListAdd">+</div>
        </div>
        <div id="modalRecentContainer">
          <div id="modalRecentHeader">
            <div id="modalRecentHeaderText">
              <%= tr('Recent ' + metadata.pluralName + ':') %>
            </div>
            <div id="modalRecentHeaderIcon">
              <svg class="modal-svg-normal" width="17" height="17" viewBox="0 0 17 17" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M11 16L11 1M14.3333 16L2.66667 16C1.74619 16 1 15.2538 1 14.3333L1 2.66667C1 1.74619 1.74619 1 2.66667 1L14.3333 1C15.2538 1 16 1.74619 16 2.66667V14.3333C16 15.2538 15.2538 16 14.3333 16Z" stroke="#4f4f4f" stroke-opacity="0.5" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" />
              </svg>
            </div>
            <button id="modalRecentHide">
              <svg class="modal-svg-toggle" width="6" height="17" viewBox="0 0 6 17" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M 1 12.5 L 5 8.5 L 1 4.5" stroke="#7b7b7b" stroke-opacity="0.5" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" />
              </svg>                
            </button>
          </div>
          <div id="modalRecentContent">
            <ul id="modalRecentWrapper">
              <% recent.forEach((item) => { %>
                <li class="modal-recent-item" data-id="<%= items.find(metadata.findPredicate(item)).id %>">
                  <div class="modal-recent-icon-left"></div>
                  <div class="modal-recent-text modal-text-nowrap modal-text-<%= metadata.color %>" data-title="<%= item.name %>">
                    <%= metadata.template({ ...item, isRecent: true }) %>
                  </div>
                  <div class="modal-recent-icon-right">
                    <svg class="modal-svg-normal" width="20" height="20" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                      <path d="M12.5 5.583H15a4.167 4.167 0 010 8.334h-2.5m-5 0H5a4.167 4.167 0 010-8.334h2.5M6.667 9.75h6.666" stroke="#E3E3E3" stroke-opacity=".5" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" />
                    </svg>                
                  </div>
                </li>
              <% }); %>
            </ul>
            <button id="modalRecentShow">
              <svg class="modal-svg-toggle" width="6" height="17" viewBox="0 0 6 17" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M 1 12.5 L 5 8.5 L 1 4.5" stroke="#7b7b7b" stroke-opacity="0.5" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" />
              </svg>                
            </button>
          </div>
        </div>
      </div>
      <% if (options.length && items.length) { %>
        <div id="modalOptionsContainer">
          <% options.forEach((option) => { %>
            <div class="modal-option" data-toggle="tooltip" data-placement="top" title="<%= tr(option.description) %>">
              <div class="modal-option-switch">
                <input type="checkbox" id="<%= option.id %>" <%= option.checked ? 'checked' : '' %> /> 
                <label for="<%= option.id %>"></label>
              </div>
              <div class="modal-option-text">
                <%= tr(option.text) %>
              </div>
            </div>
          <% }); %>
        </div>
      <% } %>
    </div>
  `),

  createResource: _.template(`
    <div class="modal" role="dialog" id="createResourceModal">
      <div class="modal-dialog" role="document">
        <div class="modal-content">
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close">
              <span aria-hidden="true">&times;</span>
            </button>
            <h4 class="modal-title"><%= tr('Create Resource') %></h4>
          </div>
          <div class="modal-body" style="padding-top: 5px;">
            <div style="color: gray; font-size: small; margin-top: 10px;">
              <%= tr('You can create resource using the "+ Create New Resource" button, which is located on the right panel') %>
              (<a href="#" onclick="BrowserAutomationStudio_OpenUrl(tr('https://i.imgur.com/03EyXLL.png')); return false;"><%= tr('screen') %></a>).
            </div>
          </div>
          <div class="modal-footer">
            <button type="button" class="btn btn-default standartbutton" data-dismiss="modal">
              <%= tr('Cancel') %>
            </button>
          </div>
        </div>
      </div>
    </div>
  `),

  createFunction: _.template(`
    <div class="modal" role="dialog" id="createFunctionModal">
      <div class="modal-dialog" role="document">
        <div class="modal-content">
          <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close">
              <span aria-hidden="true">&times;</span>
            </button>
            <h4 class="modal-title"><%= tr('Create Function') %></h4>
          </div>
          <div class="modal-body" style="padding-top: 5px;">
            <div style="color: gray; font-size: small; margin-top: 10px;">
              <%= tr('To create a new function use the function manager') %>
              (<a href="#" onclick="BrowserAutomationStudio_OpenUrl(tr('https://i.imgur.com/bsQ0SMY.png')); return false;"><%= tr('screen') %></a>).
            </div>
          </div>
          <div class="modal-footer">
            <button type="button" class="btn btn-default standartbutton" data-dismiss="modal">
              <%= tr('Cancel') %>
            </button>
          </div>
        </div>
      </div>
    </div>
  `),

  recentVariablesList: _.template(`
    <% const id = $element.data('result-target'), collection = BasDialogsLib.utils.getRecentCollection('variable', $element); %>
    <% for (let i = 0; i < Math.min(3, collection.length); ++i) { %>
      <% const displayName = BasDialogsLib.utils.getDisplayName(collection[i], 'variable'); %>
      <li class="recent-variable">
        <a tabindex="-1" href="#" data-display-name="[[<%= displayName %>]]" data-name="<%= collection[i].name %>" data-result="<%= id %>">
          <span style="margin-left: 15px; color: #bcbcbc !important;">[[<%= _.trunc(displayName, 48) %>]]</span>
        </a>
      </li>
    <% } %>
  `),

  recentResourcesList: _.template(`
    <% const id = $element.data('result-target'), collection = BasDialogsLib.utils.getRecentCollection('resource', $element); %>
    <% for (let i = 0; i < Math.min(3, collection.length); ++i) { %>
      <% const displayName = BasDialogsLib.utils.getDisplayName(collection[i], 'resource'); %>
      <li class="recent-resource">
        <a tabindex="-1" href="#" data-display-name="{{<%= displayName %>}}" data-name="<%= collection[i].name %>" data-result="<%= id %>">
          <span style="margin-left: 15px; color: #bcbcbc !important;">{{<%= _.trunc(displayName, 48) %>}}</span>
        </a>
      </li>
    <% } %>
  `),

  variablesContent: _.template(`
    <% if (global) { %>
      <span class="modal-list-prefix" title="<%= tr('Global variable') %>">G</span>
    <% } %>
    <%= BasDialogsLib.templates.visibleName({ name, isRecent }) %>
  `),

  resourcesContent: _.template(`
    <%= BasDialogsLib.templates.visibleName({ name, isRecent }) %>
  `),

  functionsContent: _.template(`
    <%= BasDialogsLib.templates.visibleName({ name, isRecent }) %>
  `),

  visibleName: _.template(`
    <span class="modal-list-name">
      <% if (!isRecent) { %>
        <b><%= name[0] %></b><%= name.slice(1) %>
      <% } else { %>
        <%= name %>
      <% } %>
    </span>
  `),
};
