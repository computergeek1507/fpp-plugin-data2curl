

<script type='text/javascript'>
function gobutton() {
    var URL = "/api/plugin-apis/sendconfig/true";
    $.get(URL);
}

</script>
<style>
</style>


<div id="global" class="settings">
<fieldset>
<legend>FPP Tasmota Plugin</legend>

<h3>Loaded  Configs</h3>
<div>
 <?php
 $fp = fopen("/home/fpp/media/config/fpp-tasmota-plugin", "r") or die("error");
 $tasmotaBulbs = array();
 while (!feof($fp)){
  $line = fgets($fp);
  if (strlen($line) > 1) { 
  list($ip, $start, $size) = explode(",", $line);
  $tasmotaBulbs[] = array($ip, $start);
  }
 }
 fclose($fp);
 print "<table border='1' cellspacing='0' cellpadding='3'>";
 print "<tr><td>IP</td><td>Start</td></tr>";
 foreach($tasmotaBulbs as $bulb)
 {
	  print "<tr><td>{$bulb[0]}</td><td>{$bulb[1]}</td></tr>";
 }
  print "</table>";
 
 ?>
</div>

<h4>About</h4>
<p>
The FPP Tasmota plugin will send color data to a Tasmota Bulb.
<p>

</fieldset>
</div>
