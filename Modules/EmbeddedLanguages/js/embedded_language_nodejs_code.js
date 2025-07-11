<% if(variables.length > 0){ %>/*<%= variables %>*/<% } %>
_embedded(<%= name %>, "Node", <%= version %>, <%= variables_string %>, <%= timeout_value()  || 60000 %> )!