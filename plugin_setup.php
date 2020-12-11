
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
<legend>FPP data2curl Config</legend>

<script>

var data2curlConfig = <? echo json_encode($pluginJson, JSON_PRETTY_PRINT); ?>;


var uniqueId = 1;
var modelOptions = "";
function AddTopic() {
    var id = $("#data2curlTableBody > tr").length + 1;
    var html = "<tr class='fppTableRow";
    if (id % 2 != 0) {
        html += " oddRow'";
    }
    html += "'><td class='colNumber rowNumber'>" + id + ".</td><td><span style='display: none;' class='uniqueId'>" + uniqueId + "</span></td>";
    html += "<td><input type='text' minlength='1' maxlength='50' size='15' class='topic' /></td>";
    html += "<td><input type='text' minlength='1' maxlength='50' size='15' class='payload' /></td>";
    html += "<td><input type='number' value='1' min='1' max='10000000' class='startchan' />"
    html += "</tr>";
    
    $("#data2curlTableBody").append(html);

    newRow = $('#data2curlTableBody > tr').last();
    $('#data2curlTableBody > tr').removeClass('selectedEntry');
    DisableButtonClass('deleteEventButton');
    uniqueId++;

    return newRow;
}

function SaveMQTTItem(row) {
    var topic = $(row).find('.topic').val();
    var payload = $(row).find('.payload').val();
    var startchan = parseInt($(row).find('.startchan').val(),10);

    var json = {
        "topic": topic,
        "payload": payload,
        "startchannel": startchan
    };
    return json;
}

function SaveMQTTItems() {
    var data2curlConfig = [];
    var i = 0;
    $("#data2curlTableBody > tr").each(function() {
        data2curlConfig[i++] = SaveMQTTItem(this);
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
function RemoveTopic() {
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
        <input type="button" value="Save" class="buttons genericButton" onclick="SaveMQTTItems();">
        <input type="button" value="Add" class="buttons genericButton" onclick="AddTopic();">
        <input id="delButton" type="button" value="Delete" class="deleteEventButton disableButtons genericButton" onclick="RemoveTopic();">
    </td>
</tr>
</table>

<div class='fppTableWrapper fppTableWrapperAsTable'>
<div class='fppTableContents'>
<table class="fppTable" id="data2curlTable"  width='100%'>
<thead><tr class="fppTableHeader"><th>#</th><th></th><th>Topic</th><th>Payload</th><th>Start Chan</th></tr></thead>
<tbody id='data2curlTableBody'>
</tbody>
</table>
</div>
</div>

</div>
<script>

$.each(data2curlConfig, function( key, val ) {
    var row = AddTopic();
    $(row).find('.topic').val(val["topic"]);
    $(row).find('.payload').val(val["payload"]);
    $(row).find('.startchannel').val(val["startchannel"]);

});
</script>

</fieldset>
</div>
