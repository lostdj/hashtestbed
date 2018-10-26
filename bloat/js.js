document.querySelector('head').innerHTML +=
/*document.write(*/
`
<!--<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>-->
<!--<meta charset="utf-8">-->

<!-- Markdeep: -->
<style class="fallback" type="text/css">
	body
	{
		visibility:hidden;
		/*white-space:pre;*/
		/*font-family:monospace;*/
	}
</style>

<!--<script type="text/javascript">window.markdeepOptions={tocStyle:'long'};</script>-->
<!--<script type="text/javascript" src="bloat/markdeep.js"></script>-->

<!---->
<!--<link rel="stylesheet" type="text/css" href="bloat/css.css"/>-->

<!--<style type="text/css">
	body
	{
		max-width: 97%;
		//max-width: max-content;
		margin: unset;
		//padding: unset;
		padding: 10px;
	}

	// apidoc.css
	/* if screen is wide enough, put table of contents on the right side */
	/*@media screen and (min-width: 64em)
	{
		.md .longTOC, .md .mediumTOC, .md .shortTOC
		{
			max-width: 20em;
			left: 54em;
			display:block;
			position: fixed;
			top:0;
			bottom:0;
			overflow-y:scroll;
			margin-top:0;
			margin-bottom:0;
			padding-top:1em;
		}
	}*/
	/* faint border below headings */
	.md h1, .md h2, .md h3, .md h4,
	.md .nonumberh1, .md .nonumberh2, .md .nonumberh3, .md .nonumberh4
	{
		border-bottom: 1px solid rgba(0,0,0,.1);
	}

	._gtml_cloaked
	{
		visibility: hidden;
	}

	._gtml_hidden
	{
		display: none;
	}
</style>-->

<!---->
<!--<link rel="stylesheet" type="text/css" href="bloat/js-emoji/emoji.css"/>-->
`;//);

document.write(
`
<!---->
<link rel="stylesheet" type="text/css" href="bloat/css.css"/>

<!---->
<!--
<link rel="stylesheet" type="text/css" href="bloat/js-emoji/emoji.css"/>
<script type="text/javascript" src="bloat/js-emoji/emoji.js"></script>
<script type="text/javascript">
	var emoji = new EmojiConvertor();
	emoji.include_title = false;
	emoji.include_text = false;
	emoji.allow_native = false;
	emoji.use_sheet = true;
	emoji.img_sets.apple.path = 'bloat/js-emoji/';
	emoji.img_sets.apple.sheet = 'bloat/js-emoji/sheet_apple_64.png';
	emoji.img_sets.google.path = 'bloat/js-emoji/';
	emoji.img_sets.google.sheet = 'bloat/js-emoji/sheet_google_64.png';
	for(e of document.getElementsByTagName('emo'))
	// emos = document.getElementsByTagName('emo');
	// for(i = 0; i < emos.length; i++)
	{
		// e = emos[i];

		h = e.innerHTML;
		n = '';

		emoji.img_set = 'apple';
		n += emoji.replace_unified(h);
		emoji.img_set = 'google';
		n += emoji.replace_unified(h);

		// e.outerHTML = n;
		// e.insertBefore(n);
		e.innerHTML = n + '<span class="nativeemo">' + h + '</span>';
		// e.getElementsByClassName('emoji-inner')[0].innerHTML = 'asd';
		// e.innerText = e.innerHTML + '<span class="nativeemo">' + h + '</span>';
	}
	// for(e of document.getElementsByTagName('emo'))
	// 	e.outerHTML = '';
	// var output1 = emoji.replace_unified(input);

</script>
-->
`);

/*
https://encrypted.google.com/search?q=js+document+ready

https://stackoverflow.com/questions/3698200/window-onload-vs-document-ready

https://stackoverflow.com/questions/9899372/pure-javascript-equivalent-of-jquerys-ready-how-to-call-a-function-when-t

https://stackoverflow.com/questions/26107125/cannot-read-property-addeventlistener-of-null
*/
// // document.addEventListener('DOMContentLoaded', function()
// window.onload = function()
// // document.ondomcontentready = function()
// // document.ondomcontentloaded = function()
// {
// 	document.write(
// 	`
// 	<script type="text/javascript">window.markdeepOptions={tocStyle:'long'};</script>
// 	<script type="text/javascript" src="bloat/markdeep.js"></script>
// 	`);

// }
// ;
// //);

document.write(
`
<!--<script type="text/javascript">window.markdeepOptions={tocStyle:'long'};</script>-->
<script type="text/javascript">window.markdeepOptions={tocStyle:'medium'};</script>
<script type="text/javascript" src="bloat/markdeep.js"></script>
`);

// // document.addEventListener('DOMContentLoaded', function()
// window.onload = function()
// // document.ondomcontentready = function()
// // document.ondomcontentloaded = function()
// {
// 	document.write(
// 	`
// 	<!---->
// 	<link rel="stylesheet" type="text/css" href="bloat/css.css"/>

// 	<!---->
// 	<link rel="stylesheet" type="text/css" href="bloat/js-emoji/emoji.css"/>
// 	<script type="text/javascript" src="bloat/js-emoji/emoji.js"></script>
// 	<script type="text/javascript">
// 		var emoji = new EmojiConvertor();
// 		emoji.include_title = false;
// 		emoji.include_text = false;
// 		emoji.allow_native = false;
// 		emoji.use_sheet = true;
// 		emoji.img_sets.apple.path = 'bloat/js-emoji/';
// 		emoji.img_sets.apple.sheet = 'bloat/js-emoji/sheet_apple_64.png';
// 		emoji.img_sets.google.path = 'bloat/js-emoji/';
// 		emoji.img_sets.google.sheet = 'bloat/js-emoji/sheet_google_64.png';
// 		for(e of document.getElementsByTagName('emo'))
// 		// emos = document.getElementsByTagName('emo');
// 		// for(i = 0; i < emos.length; i++)
// 		{
// 			// e = emos[i];

// 			h = e.innerHTML;
// 			n = '';

// 			emoji.img_set = 'apple';
// 			n += emoji.replace_unified(h);
// 			emoji.img_set = 'google';
// 			n += emoji.replace_unified(h);

// 			// e.outerHTML = n;
// 			// e.insertBefore(n);
// 			e.innerHTML = n + '<span class="nativeemo">' + h + '</span>';
// 			// e.getElementsByClassName('emoji-inner')[0].innerHTML = 'asd';
// 			// e.innerText = e.innerHTML + '<span class="nativeemo">' + h + '</span>';
// 		}
// 		// for(e of document.getElementsByTagName('emo'))
// 		// 	e.outerHTML = '';
// 		// var output1 = emoji.replace_unified(input);

// 	</script>
// 	`);

// 	document.write(
// 	`
// 	<script type="text/javascript">window.markdeepOptions={tocStyle:'long'};</script>
// 	<script type="text/javascript" src="bloat/markdeep.js"></script>
// 	`);
// }
// ;
// //, false);
