<?php include 'sessionlogin.php'; unloggedCheck(); ?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>News</title>
    <link rel="stylesheet" type="text/css" href="css/style.css">
    <link rel="stylesheet" href="css/stylehome.css">
    <link rel="stylesheet" type="text/css" href="css/stylenews.css">
  </head>
  <body>
  <div>
  <ul>
    <li><a href="home.php">Home</a></li>
    <li><a class="active" href="news.php">News</a></li>
    <li><a href="contact.php">Contact</a></li>
    <li style="float:right; color: white;">Hi <?php echo $_SESSION['name']; ?>. <a href="modify.php">Settings</a><a href="logout.php" class="button">Logout</a></li>
  </ul><br><br><br>
  </div>

  <div id="alert" class="alert">
    <div class="background"></div>
    <div class="content">
      <div class="text-alert"><span id="alertMessage">Error: default message</span></div>
      <div class="button-alert"><a id="buttonAccept" class="button" onclick="closeError();">Accept</a></div>
    </div>
  </div>

  <input type="file" id="files" name="files[]" multiple />
  <output id="list"></output>

  <script>
    function handleFileSelect(evt) {
      var files = evt.target.files; // FileList object

      // files is a FileList of File objects. List some properties.
      var output = [];
      for (var i = 0, f; f = files[i]; i++) {
        output.push('<li><strong>', escape(f.name), '</strong> (', f.type || 'n/a', ') - ',
                    f.size, ' bytes, last modified: ',
                    f.lastModifiedDate.toLocaleDateString(), '</li>');
      }
      document.getElementById('list').innerHTML = '<ul>' + output.join('') + '</ul>';
    }

    document.getElementById('files').addEventListener('change', handleFileSelect, false);
  </script>

  </div>
  </body>
</html>
