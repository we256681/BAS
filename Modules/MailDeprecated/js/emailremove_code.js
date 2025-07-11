<%= timeout_general() %>
imap_custom_query("%base%folder","STORE " + <%= mailid %> + " +Flags \\Deleted","")!
imap_custom_query("%base%folder","EXPUNGE","")!