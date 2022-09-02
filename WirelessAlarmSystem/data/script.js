//方便本地调试
let serverAddress = '';
// let serverAddress = '';
let alert1 = $('#alert1');
let alert2 = $('#alert2');
let alert3 = $('#alert3');
let alert_mqtt = $('#alert_mqtt');
let wifiInfo = $('#wifiInfo');

function warnChange()
{
    let value = document.getElementById('warn').value;
    document.getElementById('warnNum').innerHTML = '报警范围 ' + value + 'm';
}

function warnMsChange()
{
    let value = document.getElementById('warnSec').value;
    document.getElementById('warnSecNum').innerHTML = '报警时长 ' + value + 'ms';
}

$('form').submit(function (event)
{
    event.preventDefault();
    let form = $(this);
    $.ajax({
        type: form.attr('method'),
        url: serverAddress + form.attr('action'),
        data: form.serialize(),
        async: true, //是否为异步请求，ture为异步请求，false为同步请求
        timeout: 10000,
        beforeSend: LoadFunction, //加载执行方法
        error: erryFunction, //错误执行方法
        success: succFunction //成功执行方法
    });

    function LoadFunction()
    {
        let a;
        if (form.attr('action') === 'postwifi') a = alert1;
        else if (form.attr('action') === 'postwarn') a = alert2;
        else if (form.attr('action') === 'postwarnsec') a = alert3;
        else if (form.attr('action') === 'postmqtt') a = alert_mqtt;
        else return;
        a.attr('class', 'alert alert-primary');
        a.text('正在与服务器建立连接中...');
        a.show();
    }

    function erryFunction()
    {
        let a;
        console.log('服务器未响应');
        if (form.attr('action') === 'postwifi') a = alert1;
        else if (form.attr('action') === 'postwarn') a = alert2;
        else if (form.attr('action') === 'postwarnsec') a = alert3;
        else if (form.attr('action') === 'postmqtt') a = alert_mqtt;
        else return;
        a.attr('class', 'alert alert-warning');
        a.text('服务器未响应');
        a.show();
    }

    function succFunction(d)
    {
        let a;
        console.log(d);
        if (form.attr('action') === 'postwifi')
        {
            a = alert1;
            getWifi();
        }
        else if (form.attr('action') === 'postwarn') a = alert2;
        else if (form.attr('action') === 'postwarnsec') a = alert3;
        else if (form.attr('action') === 'postmqtt') a = alert_mqtt;
        else return;
        getWifi();
        getConfig();
        a.attr('class', 'alert alert-success');
        a.text(d);
        a.show();
    }
});

function getWifi()
{
    let str1 = '';
    let str2 = '';
    $.ajax({
        type: "get",
        url: serverAddress + 'networkinfo',
        data: {},
        dataType: 'json',
        async: true, //是否为异步请求，ture为异步请求，false为同步请求
        timeout: 10000,
        beforeSend: LoadFunction, //加载执行方法
        error: erryFunction, //错误执行方法
        success: succFunction //成功执行方法
    });

    function LoadFunction()
    {
    }

    function erryFunction(e)
    {
        alert1.attr('class', 'alert alert-warning');
        str2 += '<strong>服务器未响应</strong>';
        alert1.html(str2);
        alert1.show();
        wifiInfo.hide();
    }

    function succFunction(data)
    {
        let scan = data.scan;
        let isconnect = data.sta.isconnect;
        let ssid = data.sta.ssid;
        let network = data.sta.network;
        scan.forEach(function (d)
        {
            str1 += '<option value=' + d.ssid + '>';
            // str1 += '<option value=' + d.ssid + '>' + d.bssid + '</option>';
        });
        $('#SSIDS').html(str1);
        if (isconnect)
        {
            wifiInfo.attr('class', 'alert alert-info');
            str2 += '<strong>已连接WIFI：' + ssid + '</strong><br>';
            str2 += '<strong>IP: ' + network.ip + '</strong>';
            wifiInfo.html(str2);
            wifiInfo.show();
        }
        else
        {
            wifiInfo.attr('class', 'alert alert-danger');
            str2 += '<strong>WiFi连接中断</strong>';
            wifiInfo.html(str2);
            wifiInfo.show();
        }
    }
}

function getConfig()
{
    $.ajax({
        type: "get",
        url: serverAddress + 'sysinfo',
        data: {},
        dataType: 'json',
        async: true, //是否为异步请求，ture为异步请求，false为同步请求
        timeout: 10000,
        beforeSend: LoadFunction, //加载执行方法
        error: erryFunction, //错误执行方法
        success: succFunction //成功执行方法
    });

    function LoadFunction()
    {
    }

    function erryFunction(e)
    {
    }

    function succFunction(data)
    {
        let Extent1 = data.alarm.tof1;
        let Extent2 = data.alarm.tof2;
        let AlarmMS = data.alarm.ms;
        let mqttServer = data.mqtt.server;
        let mqttPort = data.mqtt.port;
        document.getElementById('warn').value = Math.round(Extent1 / 1000);
        document.getElementById('warnSec').value = AlarmMS;
        document.getElementById('mqtt_server').value = mqttServer;
        document.getElementById('mqtt_port').value = mqttPort;
        warnChange();
        warnMsChange();
    }
}

function machine(str)
{
    $.ajax({
        type: "post",
        url: serverAddress + 'machine',
        data: str,
        dataType: 'text',
        async: true, //是否为异步请求，ture为异步请求，false为同步请求
        timeout: 10000,
        beforeSend: LoadFunction, //加载执行方法
        error: erryFunction, //错误执行方法
        success: succFunction //成功执行方法
    });

    function LoadFunction()
    {
    }

    function erryFunction()
    {
        console.log('服务器未响应命令' + str);
    }

    function succFunction(d)
    {
        if (d === str)
        {
            console.log('服务器已响应命令' + str);
        }
    }
}

getWifi();
getConfig();
setInterval(getWifi, 10000);
