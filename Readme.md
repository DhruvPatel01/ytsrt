<p>This tool is not under active development. You should use any other tools to download subtitles, like Google2SRT or youtube-dl</p>

<h1>ytsrt</h1>
<h3>a tool to download bulk subtitles from YouTube</h3>
<p><br /></p>
<h3>Index</h3>
<ul>
<li><a href="#special_thanks">Special thanks to</a></li>
<li><a href="#description">Description</a></li>
<li><a href="#compilation">How to compile ?</a></li>
<li><a href="#usage">How to use ?</a><ul>
<li><a href="#usage_syntax">Syntax</a></li>
<li><a href="#usage_options">Options</a></li>
<li><a href="#usage_on_linux">On Linux</a></li>
<li><a href="#usage_on_windows">On Windows</a></li>
</ul>
</li>
<li><a href="#known_bugs">Known Bugs</a></li>
<li><a href="#report_bugs">Report Bugs</a></li>
</ul>
<h2><a name=special_thanks></a>Special thanks to</h2>
<p>I would like to thank Google2SRT developers for creating helpful &#39;help artical&#39; on <a href="http://google2srt.sourceforge.net/en/help.html#manual_download">how to download subtitles manually</a>. This whole project is based on that artical.</p>
<h2><a name=description></a>Description</h2>
<p>ytsrt is used to download subtitles from youtube.com this program can only download subtitles if they are uploaded by video uploader. YouTube offers ASR(Automatic Speech Recognition) which this tool can&#39;t download. The main feature of this tool is that you can download subtitles from playlist or from your own created video list(see file formats). There is wonderful tool called Google2SRT which can download translated and ASR subtitles.</p>
<p>Linux users can use ytsrt more powerfully using bash scripts to generate output names of already downloaded videos, creating list of input XML files using <code>ls -1</code> command etc.</p>
<p>ytsrt can also convert XML format of Google videos in SRT format locally. This process also can be done in batch mode.</p>
<p>ytsrt uses libcurl and ezxml libraries to download and parse files respectively, although these libraries are statically linked on windows so windows users don&#39;t require to download any dependencies. Linux users will have to download and install <em>curl</em>, because on Linux ytsrt is dynamically linked. The only requirement to use ytsrt is that you should have working Internet connection to download, for conversion XML-SRT Internet is not required. Of course ytsrt assumes that you will provide valid input.</p>
<h2><a name=compilation></a>How to compile ?</h2>
<p>ytsrt is dependent on two libraries one is <em>libcurl</em> to download data from Internet, and another is <em>ezxml</em> which is used to parse that data. <em>ezxml.h</em>  and <em>ezxml.c</em> are provided with source package of ytsrt. So you don&#39;t need to download that from Internet. Although you will have to download and install <em>libcurl</em> package. On Linux you can download easily from your package manager. On windows if you are beginner or not-programmer you should google it. You can download <em>libcurl</em> from <a href="http://curl.haxx.se/download.html" title="libcurl download">this url</a>. To download <em>ezxml</em> go <a href="http://ezxml.sourceforge.net/" title="ezxml homepage">here</a></p>
<p>Once you have downloaded required data you can compile with following command.</p>
<pre><code>gcc *.c -o ytsrt -lcurl</code></pre>
<h2><a name=usage></a>How to use ?</h2>
<h4><a name=usage_syntax></a>syntax :</h4>
<pre><code>ytsrt [FLAGS] [OPTION] [ARGUMENT] [OPTION] [ARGUMENT] ...</code></pre>
<h4><a name=usage_options></a>Options :</h4>
<h6><code>-d</code>, <code>--download</code></h6>
<p>this flag tells ytsrt that you want to download subtitles from YouTube.</p>
<p><strong>NOTE:</strong> This option can&#39;t be used if -c(--convert) is used.
<br /></p>
<h6><code>-c</code>, <code>--convert</code></h6>
<p>this flag tells ytsrt to convert XML files into SRT files locally.</p>
<p><strong>NOTE:</strong> This option can&#39;t be used if -d(--download) is used.</p>
<p><br /></p>
<h6><code>-u [http://url/to/valid/video | path/to/local/file.xml]</code>,</h6>
<h6><code>--from-url=[http://url/to/valid/video | path/to/local/file.xml]</code></h6>
<p>This option tells ytsrt that you want to download(convert) only single subtitle.</p>
<p>It is good practice to use quotation marks <code>&quot;</code> to include URL because YouTube URL may contain &#39;&amp;&#39; which changes the meaning of command interpretation in bash.</p>
<p>You must give URL to valid video or local XML file.</p>
<p>Valid video URLs are :</p>
<pre><code>&quot;http://www.youtube.com/watch?v=eRsGyueVLvQ&quot;
&quot;http://youtube.com/watch?v=eRsGyueVLvQ&quot;
&quot;www.youtube.com/watch?v=eRsGyueVLvQ&quot;
&quot;youtube.com/watch?v=eRsGyueVLvQ&quot;
&quot;http://www.youtu.be/eRsGyueVLvQ&quot;
&quot;http://youtu.be/eRsGyueVLvQ&quot;
&quot;www.youtu.be/eRsGyueVLvQ&quot;
&quot;youtu.be/eRsGyueVLvQ&quot;</code></pre>
<p>Valid local URLs are :</p>
<pre><code>/any/absolute/path/to/file.xml
relative/to/current/directory/file.xml
file.xml</code></pre>
<p><strong>NOTE: </strong> if -d(--download) flag is used then this option will tell ytsrt to download video from YouTube and if -c(--convert) flag is used then this option will tell ytsrt to convert local XML file.</p>
<p><strong>NOTE: </strong> this option must <strong>NOT</strong> used with -f(--from-file) option.</p>
<p><br /></p>
<h6><code>-p</code>,<code>--from-playlist</code> [Playlistid]</h6>
<h6>-p option will take playlist id as it&#39;s argument.</h6>
<p>e.g.
for www.youtube.com/playlist?list=PLcC6hSpO26Ks0yj86cpHBTchQqV9<em>b19E
the argument will be PLcC6hSpO26Ks0yj86cpHBTchQqV9_b19E</em></p>
<p>and for www.youtube.com/playlist?list=PL8B24C31197EC371C
it will be <em>PL8B24C31197EC371C</em>
you can use -R with -p option. if not used then name will be generated
automatically.</p>
<p>you don&#39;t require to use <code>--download</code> with <code>--from-playlist</code>.</p>
<p><br /></p>
<h6><code>-i [all | once | no]</code>,</h6>
<h6><code>--interacrive=[all | once | no]</code></h6>
<p>interactive option tells ytsrt the way to select subtitles.</p>
<p>for example
if <code>-i all</code> is given then ytsrt will ask you all time to select
your desired language to download. </p>
<p>e.g.</p>
<pre><code>&quot;following subtitles found, please select any one to download&quot;
1  English
2  Hindi

Your choice :</code></pre>
<p>ytsrt will ask you this question even if only one subtitle is available if <code>-i all</code> is given.</p>
<p>if <code>-i once</code> is given ytsrt will ask you first time to which subtitle download, it will remember your choice for future. In future if selected subtitle doesn&#39;t exist then it will ask you again.</p>
<p><strong>NOTE</strong>: <code>-i all</code> and <code>-i once</code> are equivalent for <code>-u</code> option.</p>
<p>if <code>-i no</code> is given and <code>--default-lang</code> is <em>not</em> given then ytsrt
will try to download English subtitle, if it is not available,
then it will switch to <code>--interactive once</code> mode.
If <code>--default-language</code> <em>is set</em> then it will try to download subtitle
in that language if it can&#39;t find that subtitle then it will switch to <code>--interactive once</code> mode.</p>
<p>it is recommended to use <code>--interactive once</code> instead of <code>--default-lang</code>.</p>
<p><br /></p>
<h6><code>-r output.srt</code>,</h6>
<h6><code>--rename-single=output.srt</code></h6>
<p>this tells ytsrt to give outputname of SRT file.</p>
<p><em><strong>-r MUST be used with <code>-u</code>(<code>--from-url</code>) and  NOT with <code>-f</code>(<code>--from-file</code>) and <code>-p</code>(<code>--playlist</code>)</strong></em></p>
<p>To download in particular directory you should provide name with base directory.</p>
<p>e.g. </p>
<pre><code>ytsrt --download \
      --from-url &quot;http://www.youtube.com/watch?v=eRsGyueVLvQ&quot; \
        --rename-single &quot;/home/user/subtitles/sintel.srt&quot; </code></pre>
<p>if <code>-r</code> is not given then on <code>-c</code> it will rename input.xml to input.srt on <code>-d</code> it will rename to video-title.srt</p>
<p><strong>NOTE:</strong> YouTube video title may contain invalid characters for output-file like &#39;\&#39;. So on Linux ytsrt will convert all occurrences of &#39;\&#39; to &#39;|&#39; and on windows ytsrt will convert all characters that are not a-z, A-Z, &#39; &#39; ,- ,&#39;_&#39;  to _.</p>
<p><br /></p>
<h6><code>-R output@no-of-digits</code>,</h6>
<h6><code>--rename-multi output@no-of-digits</code></h6>
<p>this option will let you rename multiple files.</p>
<p>This MUST be used with <code>-f</code> or <code>-p</code> option and not be used with <code>-u</code> option.</p>
<p><strong>NOTE: here you <em>MUST NOT</em> give .srt suffix</strong></p>
<p>here suppose you gave <em>-R lec@</em> then,</p>
<p>for videos from url-list file(-f) or playlist(-p) 
first video will renamed to lec01.srt
second will renamed to lec02.srt
.
.
.
eleventh will renamed to lec11.srt</p>
<p>on the other hand if <em>lec@3</em> is given then, 
first will renamed to lec001.srt and so on.</p>
<p>if <code>-R</code> is not set on <code>-f</code> or <code>-p</code> then,
for <code>-d</code> it will rename to video-title.srt</p>
<p><strong>NOTE:</strong> YouTube video title may contain invalid characters for output-file like &#39;\&#39;. So on Linux ytsrt will convert all occurrences of &#39;\&#39; to &#39;|&#39; and on windows ytsrt will convert all characters that are not a-z, A-Z, &#39; &#39; ,- ,&#39;_&#39;  to _.</p>
<p><br /></p>
<h6><code>-x</code> ,</h6>
<h6><code>--rename-file</code></h6>
<p>this option will be helpful if you already have list file
of output files like you have for URLs.</p>
<p>e.g.
urls.lst </p>
<pre><code>http://url1
http://url2
http://url3
http://url4</code></pre>
<p>output.lst</p>
<pre><code>vid1.srt
My video
Something else</code></pre>
<p>note that here input list contains four URLs where output.lst has only three names. in such case first three videos&#39; subtitles will be renamed to vid1.srt, My video.srt, Something else.srt while fourth subtitle will get it&#39;s name automatically based on it&#39;s  YouTube video title.</p>
<p><br /></p>
<h6><code>-l</code>,</h6>
<h6><code>--default-lang</code></h6>
<p>-l takes two character long string as it&#39;s argument, this option only works in &#39;--interactive no&#39; mode. if given default code is not found then ytsrt will switch to --interactive once mode.</p>
<p>if this option is not set and and <code>--interactive no</code> is set then en is selected as default language.</p>
<p><br /></p>
<h4><a name=usage_on_linux></a>On Linux</h4>
<p>To use ytsrt on Linux follow these steps.</p>
<p><1.> Install curl if you don&#39;t have already. It may be already installed because many other tool depends on it.</p>
<p><2.> Download ytsrt binary package from <a href="https://sourceforge.net/projects/ytsrt/files/">here</a></p>
<p><3.> Open terminal and go to directory where ytsrt-0.x-bin.tar.gz is downloaded.</p>
<pre><code>#for example
cd /home/user/Downloads/</code></pre>
<p><4.> execute following commands in sequence</p>
<pre><code>sudo tar xvzf ytsrt-0.x-bin.tar.gz -C /opt/
ln -s /opt/ytsrt-0.x/ytsrt-0.x /usr/locale/bin/ytsrt</code></pre>
<p><5.> Now go to directory where you want to download subtitles.</p>
<pre><code>#for example
cd /media/Jerry/courses/algorithms/subtitles/</code></pre>
<p><6.> and then use ytsrt command to download subtitles.</p>
<pre><code>#for example
ytsrt --download --rename-multi lec@2 \
--from-playlist PLUl4u3cNGP61Oq3tWYp6V_F-5jb5L2iHb</code></pre>
<p><7.> If there are subtitles then ytsrt will download them for you all at once.</p>
<h4><a name=usage_on_windows></a>On Windows</h4>
<p>To use ytsrt on Windows follow these steps.</p>
<p><1.> Download executable from <a href="https://sourceforge.net/projects/ytsrt/files/">here</a> and then copy of(or even better create shortcut to executable and then copy) executable to <code>C:\Windows</code>. </p>
<p>You can also add path to directory which contains executable to environment variables. Read <a href="http://superuser.com/questions/284342/what-are-path-and-other-environment-variables-and-how-can-i-set-or-use-them">this article</a> for more details about environment variables.</p>
<p><2 .> Now open command prompt. (There are many ways to do it. from start menu -&gt; search -&gt; cmd, or press windows key + R and then type cmd and then enter, or start -&gt; run -&gt; cmd).</p>
<p><3.> Go to the directory where you want to download subtitles. The following commands will help you.</p>
<pre><code>D: #to go to the D drive
E: #to go to the E drive and so on
cd Jerry\courses #to go to coursed directory which is inside Jerry.
mkdir subtitles #to create folder with name subtitles in-side current directory.
dir #to list all files and folders of folder where you are at present</code></pre>
<p>may be these commands are not enough for you. In this case you should ask Google for help.</p>
<p><4.> After going in to directory where you want to download subtitles you can call ytsrt.</p>
<pre><code>ytsrt -d -u  http://www.youtube.com/watch?v=HtSuA80QTyo -r lecture1.srt</code></pre>
<p>here you called ytsrt using ytsrt which is in either windows directory or in the directory to which you have set environment variable $PATH.</p>
<p>using <code>-d</code> you are telling ytsrt that you want to download subtitle(s)</p>
<p>using <code>-u</code> you are telling ytsrt that you want to download subtitle for single url, that url is given as an argument.</p>
<p>finally <code>-r</code> tells that you want to give name &#39;lecture1.srt&#39; to the downloaded subtitle.</p>
<p><br /></p>
<h2><a name=known_bugs></a>Known Bugs</h2>
<ul>
<li>Symbols are generally replaced by hex-code.</li>
</ul>
<p>This is not bug actually, this is done while parsing phase, ezxml converts all those symbols to hexcode. In future release this problem will be solved.</p>
<ul>
<li>Program may crash if you have not provided valid input. So make sure you have provided valid input.</li>
</ul>
<p><br /></p>
