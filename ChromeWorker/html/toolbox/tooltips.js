function SetupTooltips()
{
    $('[data-toggle="tooltip"]').tooltip({trigger:'hover', placement: function (tip, element) {
        var offset = $(element).offset();
        height = $(document).outerHeight();
        width = $(document).outerWidth();
        vert = 0.5 * height - offset.top;
        vertPlacement = vert > 0 ? 'bottom' : 'top';
        horiz = 0.5 * width - offset.left;
        horizPlacement = horiz > 0 ? 'right' : 'left';
        placement = Math.abs(horiz) > Math.abs(vert) ?  horizPlacement : vertPlacement;
        return placement;
    }, template:'<div class="mytooltip" role="tooltip"><div class="mytooltip-arrow tooltip-arrow"></div><div class="mytooltip-inner tooltip-inner"></div></div>',delay: { "show": 500, "hide": 100 }})
}