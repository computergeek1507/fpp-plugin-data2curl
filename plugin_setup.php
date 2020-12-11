
<?
function returnIfExists($json, $setting) {
    if ($json == null) {
        return "";
    }
    if (array_key_exists($setting, $json)) {
        return $json[$setting];
    }
    return "";
}

function convertAndGetSettings() {
    global $settings;
        
    $cfgFile = $settings['configDirectory'] . "/plugin.data2curl.json";
    if (file_exists($cfgFile)) {
        $j = file_get_contents($cfgFile);
        $json = json_decode($j, true);
        return $json;
    }
    $j = "[]";
    return json_decode($j, true);
}

$pluginJson = convertAndGetSettings();
?>


<div id="global" class="settings">
<fieldset>
<legend>FPP Data2cURL Config</legend>

<script>

var data2curlConfig = <? echo json_encode($pluginJson, JSON_PRETTY_PRINT); ?>;


var uniqueId = 1;
var modelOptions = "";
function AddCURLItem() {
    var id = $("#data2curlTableBody > tr").length + 1;
    var html = "<tr class='fppTableRow";
    if (id % 2 != 0) {
        html += " oddRow'";
    }
    html += "'><td class='colNumber rowNumber'>" + id + ".</td><td><span style='display: none;' class='uniqueId'>" + uniqueId + "</span></td>";
    html += "<td><input type='text' minlength='7' maxlength='15' size='15' pattern='^((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$' class='ipaddress' /></td>";
    html += "<td><input type='number' value='80' min='1' max='65555' class='port' />"
    html += "<td><input type='text' value='/' minlength='1' maxlength='50' size='30' class='url' /></td>";
    html += "<td><input type='text' value='%R%,%G%,%B%' minlength='1' maxlength='100' size='30' class='message' /></td>";
    html += "<td><input type='text' value='POST' minlength='1' maxlength='10' size='10' class='posttype' /></td>";
    html += "<td><input type='text' value='text/plain' minlength='1' maxlength='50' size='15' class='contenttype' /></td>";
    html += "<td><input type='number' value='1' min='1' max='10000000' class='startchan' />"
    html += "</tr>";
    
    $("#data2curlTableBody").append(html);

    newRow = $('#data2curlTableBody > tr').last();
    $('#data2curlTableBody > tr').removeClass('selectedEntry');
    DisableButtonClass('deleteEventButton');
    uniqueId++;

    return newRow;
}

function SaveCURLItem(row) {
    var ip = $(row).find('.ipaddress').val();
    var port = parseInt($(row).find('.port').val(),10);
    var url = $(row).find('.url').val();
    var message = $(row).find('.message').val();
    var posttype = $(row).find('.posttype').val();
    var contenttype = $(row).find('.contenttype').val();
    var startchan = parseInt($(row).find('.startchan').val(),10);

    var json = {
        "ip": ip,
        "port" : port,
        "url": url,
        "message": message,
        "type": posttype,
        "contenttype": contenttype,
        "startchannel": startchan
    };
    return json;
}

function SaveCURLItems() {
    var data2curlConfig = [];
    var i = 0;
    $("#data2curlTableBody > tr").each(function() {
        data2curlConfig[i++] = SaveCURLItem(this);
    });
    
    var data = JSON.stringify(data2curlConfig);
    $.ajax({
        type: "POST",
        url: 'fppjson.php?command=setPluginJSON&plugin=data2curl',
        dataType: 'json',
        async: false,
        data: data,
        processData: false,
        contentType: 'application/json',
        success: function (data) {
           SetRestartFlag(2);
        }
    });
}


function RenumberRows() {
    var id = 1;
    $('#data2curlTableBody > tr').each(function() {
        $(this).find('.rowNumber').html('' + id++ + '.');
        $(this).removeClass('oddRow');

        if (id % 2 != 0) {
            $(this).addClass('oddRow');
        }
    });
}
function RemoveCURLItem() {
    if ($('#data2curlTableBody').find('.selectedEntry').length) {
        $('#data2curlTableBody').find('.selectedEntry').remove();
        RenumberRows();
    }
    DisableButtonClass('deleteEventButton');
}


$(document).ready(function() {
                  
    $('#data2curlTableBody').sortable({
        update: function(event, ui) {
            RenumberRows();
        },
        item: '> tr',
        scroll: true
    }).disableSelection();

    $('#data2curlTableBody').on('mousedown', 'tr', function(event,ui){
        $('#data2curlTableBody tr').removeClass('selectedEntry');
        $(this).addClass('selectedEntry');
        EnableButtonClass('deleteEventButton');
    });
});

</script>
<div>
<table border=0>
<tr><td colspan='2'>
        <input type="button" value="Save" class="buttons genericButton" onclick="SaveCURLItems();">
        <input type="button" value="Add" class="buttons genericButton" onclick="AddCURLItem();">
        <input id="delButton" type="button" value="Delete" class="deleteEventButton disableButtons genericButton" onclick="RemoveCURLItem();">
    </td>
</tr>
</table>

<div class='fppTableWrapper fppTableWrapperAsTable'>
<div class='fppTableContents'>
<table class="fppTable" id="data2curlTable"  width='100%'>
<thead><tr class="fppTableHeader"><th>#</th><th></th><th>IP</th><th>Port</th><th>URL</th><th>Message</th><th>Type</th><th>Content</th><th>Start Chan</th></tr></thead>
<tbody id='data2curlTableBody'>
</tbody>
</table>
</div>

</div>
<div>
<p>
<p>Use cURL to sent {Message} to 'http://{IP}:{Port}{URL}'. 
Leave {Message} blank for GET requests. 
<p>
<table border=1>
<tr><td colspan='2'>Replace Values In URL or Message i.e. "/cmd=%R%,%G%,%B%"</td>
</tr><tr><td>Value</td><td>Parameter</td></tr>
</tr><tr><td>1st Channel(0-255)</td><td>%R%</td></tr>
</tr><tr><td>2st Channel(0-255)</td><td>%G%</td></tr>
</tr><tr><td>3st Channel(0-255)</td><td>%B%</td></tr>
</tr><tr><td>1st Channel(0-100)</td><td>%RS%</td></tr>
</tr><tr><td>2st Channel(0-100)</td><td>%GS%</td></tr>
</tr><tr><td>3st Channel(0-100)</td><td>%BS%</td></tr>
</tr><tr><td>Hue Value(0-360)</td><td>%H%</td></tr>
</tr><tr><td>Sat Value(0-100)</td><td>%S%</td></tr>
</tr><tr><td>Int Value(0-100)</td><td>%I%</td></tr>
</tr><tr><td>1st Channel as Switch Value(ON or OFF)</td><td>%SW%</td></tr>
</table>
</div>
</div>
<script>

$.each(data2curlConfig, function( key, val ) {
    var row = AddCURLItem();
    $(row).find('.ipaddress').val(val["ip"]);
    $(row).find('.port').val(val["port"]);
    $(row).find('.url').val(val["url"]);
    $(row).find('.message').val(val["message"]);
    $(row).find('.posttype').val(val["type"]);
    $(row).find('.contenttype').val(val["contenttype"]);
    $(row).find('.startchannel').val(val["startchannel"]);

});
</script>

</fieldset>
</div>
