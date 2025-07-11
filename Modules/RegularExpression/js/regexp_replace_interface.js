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
    description: tr("Regular expression. Can contain groups. Regular expressions are applied in multiline mode, if you want to match any character including line break use following expression [\\S\\s]+. Dot .+ captures any character except line break."), examples:
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
    <%= _.template($('#input_constructor').html())({id:"ReplaceWith", description:tr("Replace with"), default_selector: "string", disable_int:true, help: 
{
	description: tr("This parameter is a string. It works like this - regexp is applied to target string, all matches are replaced with this string. It also can contain special strings: \\1, \\2, \\3, etc. \\1 will be replaced with first group for current match, \\2 will be raplced with second group and so on. See task description for example.")
}}) %>
    <%= _.template($('#variable_constructor').html())({id:"Result", description:tr("Result"), default_variable: "REPLACE_RESULT_STRING", help: 
{
	description: tr("This variable will hold resulting string after replacement.")
}}) %>
</div>
<div class="tooltipinternal">
      <div class="tr tooltip-paragraph-first-fold">Execute regular expression, replace all matches with target string.</div>
      <div class="tooltip-paragraph-fold"><span class="tr">Regular expression is convenient mechanism to process strings. It helps to search for string parts, split string, etc</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://regexone.com/');return false">tutorial</a>, <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://www.rexegg.com/regex-quickstart.html');return false">manual</a> ). </div>
      <div class="tr tooltip-paragraph-fold">You could think of regexp as some kind of mask or template, which string must match. Literally regular expression could say: 'Search in a text to find a word character sequence followed by @ symbol followed by any word character or dot character.', in regular expression language it would be \w+@[a-zA-Z\.]+ and it is very simple regexp to search for mailbox.</div>

      <div class="tr tooltip-paragraph-fold">This action works like this - regexp is applied to target string, all matches are replaced with string from 'Replace with' parameter.</div>

      <div class="tooltip-paragraph-fold">
        <span class="tr">For example, if line is</span> 
        <span style="color:black">Random text login1@yahoo.com login2@outlook.com</span> 
        <span class="tr">and regexp is</span> 
        <span style="color:black">@[^\.]*\.[\s]*</span>
        <span class="tr">(this will match @ with mail domain)</span>
        <span class="tr">and replacement string is</span>
        <span style="color:black">@gmail.com</span>,  
        <span class="tr">then result will be</span>
        <span style="color:black">Random text login1@gmail.com login2@gmail.com</span>
      </div>

      <div class="tr tooltip-paragraph-fold">Variable specified in 'Result' parameter will have a string with result.</div>

      <div class="tr tooltip-paragraph-fold">Replacement string may contain parts of match by using special special strings: \1, \2, \3, etc. \1 will be replaced with first group for current match, \2 will be replaced with second group and so on.</div>

      <div class="tooltip-paragraph-fold">
        <span class="tr">Another example, if line is</span> 
        <span style="color:black">word1 test:111 word2 word3 test:222</span> 
        <span class="tr">and regexp is</span> 
        <span style="color:black">test:(\d+)</span>
        <span class="tr">(note that regular expression has one group)</span>
        <span class="tr">and replacement string is</span>
        <span style="color:black">\1</span>,  
        <span class="tr">then result will be</span>
        <span style="color:black">word1 111 word2 word3 222</span>
        <span class="tr">(\1 is replaced with first group)</span>.

      </div>


      <div class="tr tooltip-paragraph-fold">Processing html is much easier and faster with 'Xpath' module.</div>
      <div class="tooltip-paragraph-last-fold"><a href="#" onclick="BrowserAutomationStudio_OpenUrl('https://bablosoft.github.io/RegexpConstructor/'); return false;"><span class="tr">Regular Expression Constructor</span></a> <span class="tr">can help you to create regexp.</span></div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>
