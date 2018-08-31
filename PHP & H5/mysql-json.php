<?php

// $_POST['name']='100'

 // header("Content-type:text/html;charset=utf-8");


$con = mysqli_connect('127.0.0.1','root','123456','mqtt');

$sql = "SELECT TIME,DATA from mqtt;";
$sth=mysqli_query($con,$sql);
$TIME = array();
$TIME['name'] = 'TIME';
while($r = mysqli_fetch_array($sth)) {
    $TIME['data'][] = $r['DATA'].' '.$r['TIME'];
}

$sql = 'SELECT TEMP from mqtt;';
$sth=mysqli_query($con,$sql);
$TEMP = array();
$TEMP['name'] = 'TEMP';
// $TEMP['type'] = 'area';
while($r = mysqli_fetch_array($sth)) {
    $TEMP['data'][] = $r['TEMP'];
}

$sql = 'SELECT HUMI from mqtt;';
$sth=mysqli_query($con,$sql);
$HUMI = array();
$HUMI['name'] = 'HUMI';
// $HUMI['type'] = 'area';
while($r = mysqli_fetch_array($sth)) {
    $HUMI['data'][] = $r['HUMI'];
}


$result = array();
array_push($result,$TIME);
array_push($result,$TEMP);
array_push($result,$HUMI);


print json_encode($result, JSON_NUMERIC_CHECK);

mysqli_close($con);



// $msg='Hello,'.$_POST['name'].',your age is '.$_POST['age'].'!';
//     echo $msg;
?>