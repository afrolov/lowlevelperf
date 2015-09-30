# Low-level performance optimization

This is the slides and the C++ code examples I used to give a tech talk at TD
(Toronto-Dominion Bank).

The simplest way to view the slides is to use PDF file lowlevelperf.pdf

The slides are base on [reveal.js](http://lab.hakim.se/reveal-js/#/). 
To view the presentation with reveal.js, you need to: 
1. Install [node.js](https://nodejs.org/en/) and [NPM](https://www.npmjs.com/)
2. Install grunt command-line tools globally:
<pre>
npm install -g grunt-cli
</pre>
3. Clone the lowleverperf git repository
<pre>
   git clone https://github.com/afrolov/lowlevelperf
</pre>
4. Install server and client tools in lowlevelperf
<pre>
cd lowlevelperf
npm install
bower install
</pre>
5. Run the local server
<pre>
grunt serve
</pre>
6. Grunt will start the local server and open your default browser with the URL
   http://localhost:9000/ where you can browse the slides
