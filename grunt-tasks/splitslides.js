module.exports = function(grunt) {
  // Please see the Grunt documentation for more information regarding task
  // creation: http://gruntjs.com/creating-tasks
  var path = require('path');

  grunt.registerMultiTask('splitslides', 'Split markdown file into several slides.', function() {
    // Merge task-specific and/or target-specific options with these defaults.
    var options = this.options({ });    
    var jsonContent = "[\n";
    // Iterate over all specified file groups.
    this.files.forEach(function(f) {
      var self = this;
      console.log(f)
      var suffix = options.suffix;
      // Concat specified files.
      var src = f.src.filter(function(filepath) {
        // Warn on and remove invalid source files (if nonull was set).
        if (!grunt.file.exists(filepath)) {
          grunt.log.warn('Source file "' + filepath + '" not found.');
          return false;
        } else {
          return true;
        }
      }).map(function(filepath) {
        if (! suffix) {
          suffix = filepath.replace(/^.*(\.[^.]+)$/, "$1");
        }
        // Read file source.
        return grunt.file.read(filepath);
      }).join("");
      // Write the destination file.
      var fragments = src.split(options.separator);
      for (var index in fragments) {
        var destname = path.dirname(f.dest) + "/" + options.prefix + "_" + index + suffix;
        grunt.file.write(destname, fragments[index]);
        // Print a success message.
        grunt.log.writeln('File "' + destname + '" created.');
        jsonContent += '"' +  path.basename(destname) + '"'
        if (parseInt(index) + 1 !== fragments.length) {
          jsonContent += ',';
        }
        jsonContent += "\n";
      }
    });
    jsonContent += "]\n";
    grunt.file.write('slides/list.json', jsonContent);
  });
};