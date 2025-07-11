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
    <%= _.template($('#variable_constructor').html())({id:"Result", description:tr("Match list"), default_variable: "SCAN_RESULT_LIST", help: 
{
    description: tr("This variable will contain list with all matches. If group with name 'result' is present in regular expression, then list will have content of 'result' groups instead of whole matches. See task description for examples.")
}}) %>
    
</div>
    <div class="tooltipinternal">
          <div class="tr tooltip-paragraph-first-fold">Execute regular expression, save all matches into variable with list type.</div>
          <div class="tooltip-paragraph-fold"><span class="tr">Regular expression is convenient mechanism to process strings. It helps to search for string parts, split string, etc</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://regexone.com/');return false">tutorial</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.rexegg.com/regex-quickstart.html');return false">manual</a> ). </div>
          <div class="tr tooltip-paragraph-fold">You could think of regexp as some kind of mask or template, which string must match. Literally regular expression could say: 'Search in a text to find a word character sequence followed by @ symbol followed by any word character or dot character.', in regular expression language it would be \w+@[a-zA-Z\.]+ and it is very simple regexp to search for mailbox.</div>

          <div class="tr tooltip-paragraph-fold">Variable specified in 'Match list' parameter will be filled with a list, which contains every found match.</div>
          <div class="tooltip-paragraph-fold"><span class="tr">For example, if string is</span> <span style="color:black">abc 123 xyz 456</span> <span class="tr">and regexp is</span> <span style="color:black">\d+</span> <span class="tr">then it will extract all numbers found in a text, and resulting list will be</span> <span style="color:black">["123", "456"]</span></div>

              <div class="tooltip-paragraph-fold"><span class="tr">Each result list item will contain whole match, if you want to capture only part, put it in the group with name</span> <span style="color:black">result</span> <span class="tr">For example</span>, <span style="color:black">(?P&lt;result&gt;\d+)</span>.</div>

        <div class="tooltip-paragraph-fold">
            <span class="tr">If line is</span> 
            <span style="color:black">-John 100-, -Jack 200-</span> 
            <span class="tr">and regexp is</span> 
            <span style="color:black">(\w+) (?P&lt;result&gt;\d+)</span>, 
            <span class="tr">than result will contain list with 2 numbers</span>.
            <span class="tr">If regexp is</span> 
            <span style="color:black">(?P&lt;result&gt;\w+) (\d+)</span>, 
            <span class="tr">than result will contain list with 2 names</span>.
        </div>

          <div class="tr tooltip-paragraph-fold">Processing html is much easier and faster with 'Xpath' module.</div>
          <div class="tr tooltip-paragraph-fold">This action searches for all results, that matches regexp, for example it could search for all numbers in text. If you want to find only first string that matches regexp, i.e. first number, you should use 'First match' action.</div>
          <div class="tooltip-paragraph-last-fold"><a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.github.io/RegexpConstructor/'); return false;"><span class="tr">Regular Expression Constructor</span></a> <span class="tr">can help you to create regexp.</span></div>
    </div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
