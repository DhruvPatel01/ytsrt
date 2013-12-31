+-------------------------------------------------------+
|	ytsrt : to download subtitles from YouTube.			|
|														|
|			  (if available- ASR not supported)			|
+-------------------------------------------------------+

Description :
		ytsrt is used to download subtitles from youtube.com
    this program can only download subtitles if they are uploaded
    by video uploader. YouTube offers ASR(Automatic Speech-
    Recognition) which this tool can't download. The main feature
    of this tool is that you can download subtitles from playlist
    or from your own created video list(see file formats). There 
    is wonderful tool called Google2SRT which can download translated
    and ASR subtitles.

    	Linux users can use ytsrt more powerfully using bash scripts
    to generate output names of already downloaded videos, creating
    list of input XML files using `ls -1` command etc.

    	ytsrt can also convert XML format of Google videos in SRT
    format locally. this process also can be done in batch mode.

    	ytsrt uses libcurl and ezxml libraries to download and parse
    files respectively, although these libraries are statically linked
    so users don't require to download any dependencies. The only 
    requirement to use ytsrt is that you should have working Internet-
    connection to download, for conversion XML-SRT Internet is not
    required. Of course ytsrt assumes that you will provide valid input.


Syntax: ytsrt [OPTIONS]

Options :

	-d, --download
		this flag tells ytsrt that you want to download subtitles
		from you tube.

		NOTE: This option can't be used if -c(--convert) is used.

	-c,	--convert
		this flag tells ytsrt to convert XML files into SRT files
		locally.

		NOTE: This option can't be used if -d(--download) is used.


	-u [http://url/to/valid/video | path/to/local/file.xml]
	--from-url=[http://url/to/valid/video | path/to/local/file.xml]
		this option tells ytsrt that you want to download(convert)
		only single subtitle.
		
		It is good practice to use quotation marks to include 
		URL because YouTube URL may contain '&' which changes the 
		meaning of command interpretation in bash.

		you must give URL to valid video or local XML file.

		valid video URLs are :
			"http://www.youtube.com/watch?v=eRsGyueVLvQ"
			"http://youtube.com/watch?v=eRsGyueVLvQ"
			"www.youtube.com/watch?v=eRsGyueVLvQ"
			"youtube.com/watch?v=eRsGyueVLvQ"
			"http://www.youtu.be/eRsGyueVLvQ"
			"http://youtu.be/eRsGyueVLvQ"
			"www.youtu.be/eRsGyueVLvQ"
			"youtu.be/eRsGyueVLvQ"

		valid local URLs are :
			/any/absolute/path/to/file.xml
			relative/to/current/directory/file.xml
			file.xml

		NOTE: if -d(--download) flag is used then this option
			  will tell ytsrt to download video from YouTube
			  and if -c(--convert) flag is used then this option
			  will tell ytsrt to convert local XML file.
		NOTE: this option must NOT used with -f(--from-file) option.

	-f /path/to/url/list
	--from-file /path/to/xml/list
		this option tells ytsrt to download(convert) in batch mode.

		you must give argument which is URL to local file  which may absolute
		or relative to current directory.

		the list file should be formated in this way:
			->   each line contains single valid URL
				(see valid URLs in -u(--from-url) option)

			you can generate URL List from single playlist from
			http://ayra.ch/service/youtube.php, although to download
			all subtitles from single playlist	we recommend you to 
			use -p(--playlist) option rather than -f option.

			-> you can also make list file for local XMLs
				on Linux. you can do it via bash script

				for i in *.xml;
				do
				echo "$PWD""$i" >> list.txt
				done;

				NOTE: There may(actually are) other ways to create list of paths to
					  XML files on Linux like `ls -1 *.xml`

				NOTE: make sure that every XML file in list is valid 
					  google timedtext format.
		
		NOTE: this option must NOT used with -u(--from-url) or
			  -p(--playlist) option.

	-p, --from-playlist [Playlistid]
		-p option will take playlist id as it's argument.
		e.g.
		for http://www.youtube.com/playlist?list=PLcC6hSpO26Ks0yj86cpHBTchQqV9_b19E
		then argument will be PLcC6hSpO26Ks0yj86cpHBTchQqV9_b19E

		and for http://www.youtube.com/playlist?list=PL8B24C31197EC371C
		it will be PL8B24C31197EC371C

		you can use -R with -p option. if not used then name will be generated
		automatically.

		you don't require to use --download with --from-playlist.


	-i [all | once | no]
	--interacrive=[all | once | no]
		interactive option tells ytsrt the way to select subtitles.

		for example
		if '-i all' is given then ytsrt will ask you all time to select
			your desired language to download. e.g.
			"following subtitles found, please select any one to download"
			1  English
			2  Hindi

			Your choide :

			ytsrt will ask you this question even if only one subtitle is available
			if '-i all' is given.

		if '-i once' is given ytsrt will ask you first time to which subtitle download
			it will remember your choice for future.
			in future if selected subtitle doesn't exist then it will ask you again.
			
			NOTE: -i all and -i once are equvalent for -u option.

		if '-i no' is given and --default-lang is not given then ytsrt
			will try to download English subtitle if it is not available,
			then it will switch to '--interactive once' mode.
			if --default-language is set then it will try to download subtitle
			in that language if it can't find that subtitle then it will 
			to '--interactive once' mode.

			it is recommended to use '--interactive once' instead of --default-lang.

	-r output.srt
	--rename-single=output.srt
		rename tells ytsrt to give outputname of SRT file.
		-r MUST be used with -u(--from-url) and 
		   NOT with -f(--from-file) and -p(--playlist)

		To download in particular directory you should provide name
		    with base directory.
		    e.g. ytsrt --download --from-url "http://www.youtube.com/watch?v=eRsGyueVLvQ" \
		    		--rename-single "/home/user/subtitles/sintel.srt"


		if -r is not given then 
			on -c it will rename input.xml to input.srt
			on -d it will rename to video-title.srt

				NOTE: YouTube video title may contain invalid 
					characters for output-file like \
					so on Linux ytsrt will convert all occurrences of '\'
							to '|'
					and on windows ytsrt will convert all characters 
						that are not a-z,A-Z,' ',-,_ to _.

	-R output@no-of-digits
	--rename-multi output@no-of-digits
		this option will let you rename multiple files
		this MUST be used with -f or -p option
			and not be used with -u option.

		NOTE: here you MUST NOT give .srt suffix

		here suppose you gave -R lec@2 then 
			for videos from url-list file(-f) or playlist(-p) 
				first video will renamed to lec01.srt
				second will renamed to lec02.srt
				.
				.
				.
				eleventh will renamed to lec11.srt

				on the other hand if lec@3 is given
				then first will renamed to lec001.srt and so on.

		if -R is not set on -f or -p then for
		on -c it will rename input.xml to input.srt
		on -d it will rename to video-title.srt

			NOTE: YouTube video title may containt invalid 
				characters for outputfile like \
				so on Linux ytsrt will convert all occurances of \
				to |
				and on windows ytsrt will convert all characters 
				that are not a-z,A-Z,' ',-,_ to _.

		NOTE: --rename-single(-r) and --rename-multi(-R) are NOT
			supported in --convert(-c) mode.

	-x
	--rename-file
		this option will be helpful if you already have list file
		of output files like you have for URLs.

		e.g.
		urls.lst 
		http://url1
		http://url2
		http://url3
		http://url4

		output.lst
		vid1.srt
		My video
		Something else

		note that here input list contains four URLs where output.lst
		has only three names. in such case first three videos' subtitles
		will be renamed to vid1.srt, My video.srt, Something else.srt while
		fourth subtitle will get it's name automatically based on it's 
		YouTube video title.

	-l
	--default-lang
	-l takes two character long string as it's argument, this option only
		works in '--interactive no' mode. if given default code is not found
		then ytsrt will switch to --interactive once mode.

		if this option is not set and and --interactive no is set then 
		en is selected as default language.