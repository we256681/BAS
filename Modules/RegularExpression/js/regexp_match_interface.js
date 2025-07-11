<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Value", description:tr("String To Apply Regexp"), default_selector: "string", disable_int:true, help: 
{
    description: tr("This value is a target for regular expression. Could be any string.")
}}) %>
    <%= _.template($('#input_constructor').html())({id:"Regexp", description:tr("Regular Expression"), default_selector: "string", disable_int:true,variants: [

"\\d+<br/><span style='color:gray;font-size:small'>" + tr("Any digit sequence") + "</span>",
"\\w+<br/><span style='color:gray;font-size:small'>" + tr("Any word character sequence") + "</span>",
"\\s+<br/><span style='color:gray;font-size:small'>" + tr("Any whitespace character sequence (includes tabs and line breaks)") + "</span>",
".+<br/><span style='color:gray;font-size:small'>" + tr("Any number of any characters except line break") + "</span>",
"[\\S\\s]+<br/><span style='color:gray;font-size:small'>" + tr("Any number of any characters including line break") + "</span>",
"[a-z]+<br/><span style='color:gray;font-size:small'>" + tr("Any lowercase character sequence") + "</span>",
"[0-9]+<br/><span style='color:gray;font-size:small'>" + tr("Any non digit sequence") + "</span>",

"[^0-9]+<br/><span style='color:gray;font-size:small'>" + tr("Any non digit sequence") + "</span>",

"Your user name is: [^\\s]+<br/><span style='color:gray;font-size:small'>" + tr("'Your user name is: ' string than any number of non whitespace character") + "</span>",
"Your user name is: ([^\\s]+)<br/><span style='color:gray;font-size:small'>" + tr("Save name into GROUP1 variable by using group") + "</span>",
"href=\"([^\"]+)\"<br/><span style='color:gray;font-size:small'>" + tr("Extract link from html a tag") + "</span>"
],

     help: 
{
    description: tr("Regular expression. Regular expressions are applied in multiline mode, if you want to match any character including line break use following expression [\\S\\s]+. Dot .+ captures any character except line break."), examples:
    [
        {
            code:"\\d+",description:tr("Any digit sequence")
        },
        {
            code:"\\w+",description:tr("Any word character sequence")
        },
        {
            code:"\\s+",description:tr("Any whitespace character sequence (includes tabs and line breaks)")
        },
        {
            code:".+",description:tr("Any number of any characters except line break")
        },
        {
            code:"[\\S\\s]+",description:tr("Any number of any characters including line break")
        },
        {
            code:"[a-z]+",description:tr("Any lowercase character sequence")
        },
        {
            code:"[0-9]+",description:tr("Any digit sequence")
        },
        {
            code:"[^0-9]+",description:tr("Any non digit sequence")
        },
        {
            code:"Your user name is: [^\\s]+",description:tr("'Your user name is: ' string than any number of non whitespace character")
        },
        {
            code:"Your user name is: ([^\\s]+)",description:tr("Save name into GROUP1 variable by using group")
        },
        {
            code:"href=\"([^\"]+)\"",description:tr("Extract link from html a tag")
        }
    ]
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Result", description:tr("If string matches"), default_variable: "STRING_MATCHES", help: 
{
	description: tr("This variable will have true or false value depending if string matches regular expression.")
}}) %>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Test if string matches regular expression, return result into variable.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Regular expression is convenient mechanism to process strings. It helps to search for string parts, split string, etc</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://regexone.com/');return false">tutorial</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.rexegg.com/regex-quickstart.html');return false">manual</a> ). </div>
      <div class="tr tooltip-paragraph-fold">You could think of regexp as some kind of mask or template, which string must match. Literally regular expression could say: 'Search in a text to find a word character sequence followed by @ symbol followed by any word character or dot character.', in regular expression language it would be \w+@[a-zA-Z\.]+ and it is very simple regexp to search for mailbox.</div>

      <div class="tr tooltip-paragraph-fold">Variable specified in 'If string matches' parameter will have true or false value depending if string matches regular expression.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">For example, if string is</span> <span style="color:black">test string</span> <span class="tr">and regexp is</span> <span style="color:black">[a-z]</span> <span class="tr">then this action will return true, because string contains lowercase character, but regexp </span> <span style="color:black">[A-Z]</span> <span class="tr">will return false, because string doesn't contain any uppercase character</span>.</div>


      <div class="tr tooltip-paragraph-fold">Processing html is much easier and faster with 'Xpath' module.</div>
      <div class="tooltip-paragraph-last-fold"><a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.github.io/RegexpConstructor/'); return false;"><span class="tr">Regular Expression Constructor</span></a> <span class="tr">can help you to create regexp.</span></div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
