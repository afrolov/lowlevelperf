# Low-level performance optimization

This is the slides and the C++ code examples I used to give a tech talk at TD
(Toronto-Dominion Bank).

The simplest way to view the slides is to use PDF file lowlevelperf.pdf

The slides are base on [reveal.js](http://lab.hakim.se/reveal-js/#/). 
To view the presentation with reveal.js, you need to: 
1. Install [node.js](https://nodejs.org/en/) and [NPM](https://www.npmjs.com/)
2. Install grunt command-line tools globally:
  ````
  npm install -g grunt-cli
  ````
3. Clone the lowleverperf git repository
   ````
   git clone https://github.com/afrolov/lowlevelperf
   ````
3. Install server and client tools in lowlevelperf
   ````
   cd lowlevelperf
   npm install
   bower install
   ````
4. Run the local server
   ````
   grunt serve
   ````

5. Grunt will start the local server and open your default browser with the URL
   http://localhost:9000/ where you can browse the slides
