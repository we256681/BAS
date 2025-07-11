<div class="container-fluid">
    <%= _.template($('#input_constructor').html())({id:"Value", description:tr("String To Apply Regexp"), default_selector: "string", disable_int:true, help: 
{
    description: tr("This value is a target for regular expression. Could be any string.")
}}) %>
    <%= _.template($('#input_constructor').html())({id:"Regexp", description:tr("Regular Expression"), default_selector: "string", disable_int:true

,variants: [

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
    description: tr("Regular expression. Can contain groups, all captured groups will be placed into variables. Regular expressions are applied in multiline mode, if you want to match any character including line break use following expression [\\S\\s]+. Dot .+ captures any character except line break."), examples:
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
    <%= _.template($('#variable_constructor').html())({id:"Result", description:tr("Captured groups"), default_variable: "GROUP1,GROUP2,GROUP3", append_mode:true, help: 
{
    description: tr("List of variable names separated by comma. Each variable will contain one captured group. To set group in regular expression, use brackets. By default first group will be placed into GROUP1 variable, second - into GROUP2 and third into GROUP3")
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"ResultAll", description:tr("All string that matches regexp"), default_variable: "ALL_MATCH", help: 
{
    description: tr("All text captured by regular expression.")
}}) %>
    
</div>


    <div class="tooltipinternal">
          <div class="tr tooltip-paragraph-first-fold">Execute regular expression, get first match and captured groups.</div>
          <div class="tooltip-paragraph-fold"><span class="tr">Regular expression is convenient mechanism to process strings. It helps to search for string parts, split string, etc</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://regexone.com/');return false">tutorial</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.rexegg.com/regex-quickstart.html');return false">manual</a> ). </div>
          <div class="tr tooltip-paragraph-fold">You could think of regexp as some kind of mask or template, which string must match. Literally regular expression could say: 'Search in a text to find a word character sequence followed by @ symbol followed by any word character or dot character.', in regular expression language it would be \w+@[a-zA-Z\.]+ and it is very simple regexp to search for mailbox in text.</div>

          <div class="tr tooltip-paragraph-fold">Variable specified in 'Entire match' parameter will be filled with whole matching text.</div>
          <div class="tooltip-paragraph-fold"><span class="tr">If you want to capture only part of entire match, then you should use groups. Groups are created by adding brackets into regular expression. For example, if you want to process following string</span> <span style="color:black">Your user name is: test</span> <span class="tr">and capture string</span> <span style="color:black">test</span>, <span class="tr">you can use following regular expression</span> <span style="color:black">Your user name is: ([^\s]+)</span> </div>
          <div class="tooltip-paragraph-fold">
            <span class="tr">Several groups may be captured. If line is</span> 
            <span style="color:black">-John 100-</span> 
            <span class="tr">and regexp is</span> 
            <span style="color:black">(\w+) (\w+)</span>, 
            <span class="tr">than variable</span>
            <span style="color:green">ALL_MATCH</span> 
            <span class="tr">will be equal to</span> 
            <span style="color:black">John 100</span>, 
            <span style="color:green">GROUP1</span> 
            <span class="tr">will be equal to</span> 
            <span style="color:black">John</span> 
            <span class="tr">and</span> 
            <span style="color:green">GROUP2</span> 
            <span class="tr">will be equal to</span>
            <span style="color:black">100</span>.
          </div>
          <div class="tr tooltip-paragraph-fold">Processing html is much easier and faster with 'Xpath' module.</div>
          <div class="tr tooltip-paragraph-fold">This action searches only for the first string, that matches regexp, for example it could search for first number in text. If you want to find several strings that matches regexp, i.e. all numbers, you should use 'Exctract all data' action.</div>
          <div class="tooltip-paragraph-last-fold"><a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.github.io/RegexpConstructor/'); return false;"><span class="tr">Regular Expression Constructor</span></a> <span class="tr">can help you to create regexp.</span></div>
    </div>


    
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
