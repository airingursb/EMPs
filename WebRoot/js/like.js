$.getJSON("./show_like.action", function (data) {
    $('.like-vote span').html(data.status);
});
$('.like-vote').click(function () {
    if ($('.like-title').html() === 'Do you like me?') {
        $.getJSON("./add_like.action", function (data) {
            if (Math.random() > 0.5) {
                $('.like-vote span').html(data.status);
                $('.like-title').html('我也喜欢你 (*≧▽≦)');
            }
            else {
            	$('.like-vote span').html(data.status);
                $('.like-title').html('你的爱我已经感受到了~');
            }
        });
    }
});