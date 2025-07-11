function ExtractVariables(source)
{
  var Result = [...source.matchAll(/\[\[([^\]]+)\]\]/g)].map((match) => {
    return "VAR_" + match[0].trim()
      .replace(/\[/g, '')
      .replace(/\]/g, '')
  });
  return [...new Set(Result)]
}