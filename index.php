->
<html>
	<head>
		<meta charset="utf-8">
		<title>MQTT Client By JMK</title>
		<link rel="icon" href="https://static.jianshukeji.com/highcharts/images/favicon.ico">
		<meta name="viewport" content="width=device-width, initial-scale=1">	
	    <script src="https://img.hcharts.cn/jquery/jquery-1.8.3.min.js"></script>
		<script src="https://img.hcharts.cn/highcharts/highcharts.js"></script>
		<script src="https://img.hcharts.cn/highcharts/modules/exporting.js"></script>
		<script src="https://img.hcharts.cn/highcharts/modules/oldie.js"></script>
		<script src="https://img.hcharts.cn/highcharts-plugins/highcharts-zh_CN.js"></script>
		<script src="https://img.hcharts.cn/highcharts/themes/dark-unica.js"></script>

		<link href="http://47.106.209.211/mqtt/css/bootstrap.min.css" rel="stylesheet">
		<link href="http://47.106.209.211/mqtt/css/style.css" rel="stylesheet">
		<link href="http://47.106.209.211/mqtt/css/jsoneditor.css" rel="stylesheet" type="text/css">
        <script src="http://47.106.209.211/mqtt/js/jsoneditor.js"></script>
        
		<style type="http://47.106.209.211/mqtt/text/css">
    code {
      background-color: #f5f5f5;
    }
    #jsoneditor {
	  
      height: 300px;
    }
	
	#jsoneditor div.jsoneditor{
	  border:0px;
	}
  </style>
	</head>

	<body onload="init()">
		<div id="container1" style="min-width:400px;height:500px margin:0 50px"></div>
		<button type="button" id="clear-button">Clear Plot</button>
	    <button type="button" id="refresh-button">Refre Plot</button>
		 
       <div class="switch demo3">
			<input type="checkbox" onclick="checkboxOnclick1(this)"/>
			<label><i></i></label>
		</div>
      <!--  <div class="switch demo3">
			<input type="checkbox" onclick="checkboxOnclick2(this)"/>
			<label><i></i></label>
		</div>
		<div class="switch demo3">
			<input type="checkbox" onclick="checkboxOnclick3(this)"/>
			<label><i></i></label>
		</div> -->
		<div class="navbar navbar-inverse" role="navigation">
			<div class="container">
				<div class="navbar-header pull-left">
					<a class="navbar-brand" href="#">MQTT Helper</a>
				</div>
				<div class="navbar-header pull-right" id="connectionStatus"></div>
			</div>
		</div>
		<div class="container">
			<div class="row">
				<div class="col-xs-12">
					<div class="panel panel-success">
						<div class="panel-heading" id="connectPanelHeading">
							<h3 class="panel-title">
								<a id="connectPanelLink" data-toggle="collapse" data-target="#collapseConnect" href="#collapseConnect">
									连接
								</a>
							</h3>
						</div>
						<div id="collapseConnect" class="panel-collapse collapse">
							<div class="panel-body">
								<div class="container">
									<div class="row">
										<div class="col-md-6">
											<form class="form" id="connectForm" role="form">
												<div class="form-group">
													<label for="connectServer">Server</label>
													<input type="text" class="requiresDisconnect form-control" id="connectServer" placeholder="Server" value="messagesight.demos.ibm.com">
												</div>
												<div class="form-group">
													<label for="connectPort">Port</label>
													<input type="text" class="requiresDisconnect form-control" id="connectPort" placeholder="Port" value="1883">
												</div>
												<button type="submit" id="connectButton" class="requiresDisconnect btn btn-success btn-small">连接</button>
												<button id="disconnectButton" class="requiresConnect btn btn-danger btn-small">断开连接</button>
											</form>
										</div>
										<hr class="visible-xs visible-sm">
										<div class="col-md-6">
											<div style="opacity: 0.8">
												<div class="input-group">
													<span class="input-group-addon">Client ID</span>
													<input type="text" class="requiresDisconnect form-control" id="connectClientID" value=""> 
												</div>
												<div class="input-group">
													<span class="input-group-addon">Username</span>
													<input type="text" class="requiresDisconnect form-control" id="connectUsername" value="" placeholder="(optional)"> 
												</div>
												<div class="input-group">
													<span class="input-group-addon">Password</span>
													<input type="password" class="requiresDisconnect form-control" id="connectPassword" value="" placeholder="(optional)"> 
												</div>
												<div class="input-group">
													<span style="width: 120px" class="input-group-addon">Clean Session</span>
													<div class="btn-group btn-toggle">
														<button type="button" id="connectCleanSessionOff" class="requiresDisconnect btn btn-default">OFF</button>
														<button type="button" id="connectCleanSessionOn" class="requiresDisconnect btn btn-primary active">ON</button>
													</div>
												</div>
												<div class="input-group">
													<span style="width: 120px" class="input-group-addon">SSL</span>
													<div class="btn-group btn-toggle">
														<button type="button" id="connectSSLOff" class="requiresDisconnect btn btn-primary active">OFF</button>
														<button type="button" id="connectSSLOn" class="requiresDisconnect btn btn-default">ON</button>
													</div>
												</div>
											</div>
										</div>
									</div>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
			<div class="row">
				<div class="col-sm-6">
					<div class="panel panel-danger">
						<div class="panel-heading" id="subscribePanelHeading">
							<h3 class="panel-title">
								<a id="subscribePanelLink" data-toggle="collapse" data-target="#collapseSubscribe" href="#collapseSubscribe" class="collapsed">
									订阅
								</a>
							</h3>
						</div>
						<div id="collapseSubscribe" class="panel-collapse collapse in"> 
							<!--class="panel-collapse collapse in"> 
							<!---->
							<div class="panel-body">
								<form class="form" id="subscribeForm" role="form">
									<div class="form-group">
										<label for="subscribeTopic">Topic</label>
										<input type="text" class="requiresConnect form-control" id="subscribeTopic" value="planets/earth">
									</div>
									<div class="input-group">
										<span class="input-group-addon">QOS</span>
										<select style="width: 60px;" class="requiresConnect form-control" id="subscribeQOS">
											<option>0</option>
											<option>1</option>
											<option>2</option>
										</select>
										<button type="submit" id="subscribeButton" class="requiresConnect btn btn-success btn-small pull-right">订阅</button>
									</div>
								</form>
								<div id="subscriptionListContainer">
									<h5 id="subscribeListLabel">订阅列表</h5>
									<div id="subscribeList"></div>
								</div>
							</div>
						</div>
					</div>
				</div>
				<div class="col-sm-6">
					<div class="panel panel-info">
						<div class="panel-heading" id="publishPanelHeading">
							<h3 class="panel-title">
								<a id="publishPanelLink" data-toggle="collapse" data-target="#collapsePublish" href="#collapsePublish" class="collapsed">
									发布
								</a>
							</h3>
						</div>
						<div id="collapsePublish" class="panel-collapse collapse in">
							<div class="panel-body">
								<form class="form" id="publishForm" role="form">
									<div class="form-group">
										<label for="subscribeTopic">Topic</label>
										<input type="text" class="requiresConnect form-control" id="publishTopic" value="planets/earth">
									</div>
									<div class="input-group" style="width:100%">
										<label for="subscribeTopic">Payload</label>
										<div id="jsoneditor"></div>
										
									</div>
									<div class="input-group">
										<span class="input-group-addon">QOS</span>
										<select style="width: 60px;" class="requiresConnect form-control" id="publishQOS">
											<option>0</option>
											<option>1</option>
											<option>2</option>
										</select>
									</div>
									<div class="input-group">
										<span class="input-group-addon">Retained</span>
										<div class="btn-group btn-toggle" data-toggle="buttons-radio">
											<button type="button" id="publishRetainedOff" class="requiresConnect btn btn-primary active">OFF</button>
											<button type="button" id="publishRetainedOn" class="requiresConnect btn btn-default">ON</button>
										</div>
										<button type="submit" id="publishButton" class="requiresConnect btn btn-success btn-small pull-right">发布</button>
									</div>
								</form>
							</div>
						</div>
					</div>
				</div>
			</div>
			<div class="row">
				<div class="col-xs-12">
					<div class="panel panel-warning">
						<div class="panel-heading" id="logPanelHeading">
							<h3 class="panel-title">
								<a id="logPanelLink" data-toggle="collapse" data-target="#collapseLog" href="#collapseLog">Log</a>
									<span style="margin-left: 10px; padding: 2px 5px" class="badge badge-info" id="logSize"></span>
									<button class="btn btn-text btn-mini" style="display: inline-block; margin: 5px 5px 8x 10px;" onclick="clearLog()">Clear</button>
									<input type="checkbox" id="stickyLog" style="display: inline-block; margin: 5px 5px 8px 20px;" />Follow
								</a>
							</h3>
						</div>
						<div id="collapseLog" class="panel-collapse collapse">
							<div class="panel-body">
								<div style="" class="pre-scrollable" id="logContents"></div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
		</div>
		<div class="navbar navbar-inverse" role="navigation">
			<div class="container">
				<div class="navbar-header pull-left">
					<a class="navbar-brand" href="#"></a>
				</div>
				<div class="navbar-header pull-right" id="connectionStatus"></div>
			</div>
		</div>


		<script>
		  var container = document.getElementById('jsoneditor');

		  var options = {
			mode: 'code',
			modes: ['code','tree','text'],
			onError: function (err) {
			  alert(err.toString());
			}
		  };
		  var editor = new JSONEditor(container, options, null);
		</script>

		<script>
		var chart = null;
	    $.getJSON('http://47.106.209.211/mqtt/mysql-json.php', function (json) {
		chart = Highcharts.chart('container1', {
			
	        
			chart: {
				marginRight: 100,
                marginLeft: 80,
				type: 'spline',
				zoomType: 'x',
				panning: true,  //平移功能开启
	            panKey: 'shift',//按住shift 平移
	            marginRight: 100,
	            marginLeft: 80,
			},
			title: {
	              text: 'Cloud CW Recording temp and humi', //标题
	              x: -20
	            },
	        subtitle: {
	              text: 'By JMK',  //副标题
	              x: -20
	            },
			xAxis: {
	                text: 'timeline',
				 
				 tickInterval: 200,
	             tickPixelInterval: 1,
	              title: {
	                text: 'timeline',
	              },

	              categories: json[0]['data']
			},
			yAxis: {
				 min: 0,
	              title: {
	                text: 'Values'
	              },
	             plotLines: [{
	                value: 0,
	                width: 1,
	                color: '#808080'
	             }]
			},

			credits:{ //版权标签  显示在图表右下方的包含链接的文字，默认是Highcharts官网地址。通过指定credits.enabled=false即可不显示该信息。
	              enabled:true,
	              text:'JMK.xin',
	              href:'http://47.106.209.211/jmk/',
	              style: {
	                cursor: 'pointer',
	                color: 'green',
	                fontSize: '20px'
	              }
	        },

	        tooltip: {//数据提示框  当鼠标悬停在某点上时，以框的形式提示该点的数据，比如该点的值、数据单位等。数据提示框内提示的信息完全可以通过格式化函数动态指定。
		        formatter: function() {
		                 return '<b>'+ this.series.name +'</b><br/>'+
		                               'TIME: '+this.x +'</b><br/>'+ 
		                               'Value: '+this.y;
		               }
	        },
			legend: { ///图例  图例是图表中用不同形状、颜色、文字等 标示不同数据列，通过点击标示可以显示或隐藏该数据列。
	              layout: 'vertical',
	                align: 'right',
	                verticalAlign: 'top',
	                x: -10,
	                y: 100,
	                borderWidth: 0
	        },
			plotOptions: {
				area: {
					fillColor: {
						linearGradient: {
							x1: 0,
							y1: 0,
							x2: 0,
							y2: 1
						},
						stops: [
							[0, Highcharts.getOptions().colors[0]],
							[1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
						]
					},
					marker: {
						radius: 2
					},
					lineWidth: 1,
					states: {
						hover: {
							lineWidth: 1
						}
					},
					threshold: null
				}
			},
			series: [json[1],json[2]]
		});
	    $('#clear-button').on('click', function (e) { //点击事件
			clearPlot();
	    });

		$('#refresh-button').on('click', function (e) {
			clearPlot();
			refreshPlot();
		});

	});
			 function clearPlot() {    //清空表
					    	//console.log("clear plot data!!!");
				            var series=chart.series;	            
				            while(series.length > 0) {
				                series[0].remove(false);
				            }
				            chart.redraw();
			 };

	         function refreshPlot() {   //更新表
		     $.getJSON('http://47.106.209.211/mqtt/mysql-json.php', function (json){
			    	//console.log("refresh plot data!!!");
			    	//console.log(json);
			    	// chart.addSeries(json[0], false);
		            chart.addSeries(json[1], false);
	            	chart.addSeries(json[2], false);
	        		chart.redraw();
	            	});
		            
			    };
	</script>
		<!-- Placed at the end of the document so the pages load faster -->
		<script src="http://47.106.209.211/mqtt/js/utils/jquery.min.js"></script>
		<script src="http://47.106.209.211/mqtt/js/utils/bootstrap.min.js"></script>
		<script src="http://47.106.209.211/mqtt/js/utils/mqttws31.js"></script>
		<script src="http://47.106.209.211/mqtt/js/MQTTClient.js"></script>
		
	</body>
</html>