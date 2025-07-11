function InitUnfoldTooltip()
{
	if(!($(".tooltip-paragraph-first-fold").length > 0 && $(".tooltip-paragraph-last-fold").length > 0))
		return;
	$(".tooltip-paragraph-first-fold").append(` <a href='#' onclick='UnfoldTooltip(this); return false'>${tr('Read more')} ...</a>`);
}

function UnfoldTooltip(self)
{
	$(".tooltip-paragraph-last-fold").prepend('<i class="fa fa-angle-double-right"></i> ')
	$(".tooltip-paragraph-first-fold").prepend('<i class="fa fa-angle-double-right"></i> ')
	$(".tooltip-paragraph-fold").prepend('<i class="fa fa-angle-double-right"></i> ')

	$(".tooltip-paragraph-last-fold").removeClass("tooltip-paragraph-last-fold").addClass("tooltip-paragraph-last-unfold");
	$(".tooltip-paragraph-fold").removeClass("tooltip-paragraph-fold").addClass("tooltip-paragraph-unfold");
	$(".tooltip-paragraph-first-fold").removeClass("tooltip-paragraph-first-fold").addClass("tooltip-paragraph-first-unfold");
	$(self).remove()

}