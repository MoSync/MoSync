<?php
	/*
	TODO Show the performance trend of a runtime on different benchmarks
	*/

	require_once('db_priv_conn.php'); //set up the connection to the database

	/* GET membench results
	SELECT a.runtime_id, a.phone_id, runtime_id, (alloc_str_10 + alloc_str_100)/2 AS alloc_str, (alloc_void_1 + alloc_void_100 + alloc_void_1000)/3 AS alloc_void, (alloc_dummy + alloc_dummy_struct)/2 AS alloc_small_objects, alloc_dummy_mix AS alloc_big_objects, (access_array + access_vector)/2 AS access_array_vector, add_vector, (access_dummy + access_dummy_struct)/2 AS access_small_objects, access_dummy_mix AS access_big_objects FROM membench_testruns;
	*/

	/* store the input retrieved via HTTP GET */
	$device = mysql_real_escape_string($_GET['device']); 				//the device we want extra information about
	$runtime_id = mysql_real_escape_string($_GET['run_id']); 				//the runtime we want extra information about
	$gl_test = 'test'.mysql_real_escape_string($_GET['gl_test']); //the gl test that we want history of (1,2,3 or 4)

	/* this SQL statement returns the latest testruns of the linpack benchmark, the latest per distinct runtime */
	$result_latest_runs = mysql_query("SELECT
    phone_name, runtime_name, mflops, id
	FROM
    linpack_testruns a
    inner join
        (SELECT runtime_name, max(id) as maxid FROM linpack_testruns, runtimes WHERE linpack_testruns.runtime_id = runtimes.runtime_id group by runtime_name) as b on
        a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id");
	$num_rows = mysql_num_rows($result_latest_runs);
	$i = 0;


	/* This SQL statement get the latest runs of the membench benchmark, the latest of each runtime */
	$result_latest_membench = mysql_query("SELECT phones.*, b.* FROM membench_testruns a inner join  (SELECT runtime_name, id, (alloc_str_10 + alloc_str_100)/2 AS alloc_str, (alloc_void_1 + alloc_void_100 + alloc_void_1000)/3 AS alloc_void, (alloc_dummy + alloc_dummy_struct)/2 AS alloc_small_objects, alloc_dummy_mix AS alloc_big_objects, (access_array + access_vector)/2 AS access_array_vector, add_vector, (access_dummy + access_dummy_struct)/2 AS access_small_objects, access_dummy_mix AS access_big_objects, max(id) as maxid FROM membench_testruns, runtimes WHERE membench_testruns.runtime_id = runtimes.runtime_id group by runtime_name, id ORDER BY id DESC) as b on  a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id GROUP BY runtime_name;");
	/*mysql_query("SELECT  *
	FROM
    membench_testruns a
    inner join
        (SELECT runtime_name, max(id) as maxid FROM membench_testruns, runtimes WHERE membench_testruns.runtime_id = runtimes.runtime_id group by runtime_name) as b on
        a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id");*/

	$result_average_membench = mysql_query("SELECT runtime_name, AVG((alloc_str_10 + alloc_str_100)/2) AS alloc_str, AVG((alloc_void_1 + alloc_void_100 + alloc_void_1000)/3) AS alloc_void, AVG((alloc_dummy + alloc_dummy_struct)/2) AS alloc_small_objects, AVG(alloc_dummy_mix) AS alloc_big_objects, AVG((access_array + access_vector)/2) AS access_array_vector, AVG(add_vector) AS add_vector, AVG((access_dummy + access_dummy_struct)/2) AS access_small_objects, AVG(access_dummy_mix) AS access_big_objects FROM membench_testruns, runtimes  WHERE runtimes.runtime_id = membench_testruns.runtime_id  AND DATEDIFF( NOW( ) , TIMESTAMP ) <30  GROUP BY runtime_name;");


	// get the average LINPACK result of each runtime last month
	$result_average_runtime = mysql_query("SELECT runtime_name, AVG( mflops ) AS avgmflops
	FROM linpack_testruns, runtimes
	WHERE runtimes.runtime_id = linpack_testruns.runtime_id
	AND DATEDIFF( NOW( ) , TIMESTAMP ) <30
	GROUP BY runtime_name ORDER BY avgmflops DESC");

	// get historical result for a specific device
	$result_hist = mysql_query("SELECT runtime_name, runtimes.runtime_id as run_id, avg(mflops) as avgmflops, DATEDIFF(now(), timestamp) AS datediff
	FROM linpack_testruns, runtimes
	WHERE runtimes.runtime_id = linpack_testruns.runtime_id AND phone_id = '$device'
	GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC");

	//get the performance trend on the LINPACK benchmark on all runtimes
	$result_trend = mysql_query("");

	$result_latest_opengl = mysql_query("SELECT phone_name, runtime_name, test1, test2, test3, test4, id FROM opengl_testruns a
	inner join (SELECT runtime_name, max(id) as maxid FROM opengl_testruns, runtimes WHERE opengl_testruns.runtime_id = runtimes.runtime_id group by runtime_name)
	as b on
	a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id;");

	$result_average_opengl = mysql_query("SELECT runtime_name, avg(test1) AS test1, avg(test2) AS test2, avg(test3) AS test3, avg(test4) AS test4
	FROM opengl_testruns, runtimes  WHERE runtimes.runtime_id = opengl_testruns.runtime_id AND DATEDIFF( NOW( ) , timestamp ) <30
	GROUP BY runtime_name;");

	$result_hist_opengl = mysql_query("SELECT runtime_name, runtimes.runtime_id as run_id, avg($gl_test) as test, DATEDIFF(now(), timestamp) AS datediff
	FROM opengl_testruns, runtimes  WHERE runtimes.runtime_id = opengl_testruns.runtime_id AND runtimes.runtime_id = '$runtime_id' AND phone_id = '$device'
	GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC;");

	$result_hist_opengl_all_tests = mysql_query("SELECT runtime_name, runtimes.runtime_id as run_id, avg(test1) as test1, avg(test2) as test2, avg(test3) as test3, avg(test4) as test4, DATEDIFF(now(), timestamp) AS datediff   FROM opengl_testruns, runtimes  WHERE runtimes.runtime_id = opengl_testruns.runtime_id AND phone_id = '$device' AND runtimes.runtime_id = '$runtime_id'   GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC;
");

	$result_hist_membench = mysql_query("SELECT runtime_name, AVG((alloc_str_10 + alloc_str_100)/2) AS alloc_str, AVG((alloc_void_1 + alloc_void_100 + alloc_void_1000)/3) AS alloc_void, AVG((alloc_dummy + alloc_dummy_struct)/2) AS alloc_small_objects, AVG(alloc_dummy_mix) AS alloc_big_objects, AVG((access_array + access_vector)/2) AS access_array_vector, AVG(add_vector) AS add_vector, AVG((access_dummy + access_dummy_struct)/2) AS access_small_objects, AVG(access_dummy_mix) AS access_big_objects, DATEDIFF(now(), timestamp) AS datediff FROM membench_testruns, runtimes  WHERE runtimes.runtime_id = membench_testruns.runtime_id AND runtimes.runtime_id = '$runtime_id' GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC;
");

	/*$result_latest_membench = mysql_query("SELECT phone_name, runtime_name, alloc_str_10 + alloc_str_100 AS alloc, id FROM membench_testruns a
	inner join (SELECT runtime_name, max(id) as maxid FROM membench_testruns, runtimes WHERE membench_testruns.runtime_id = runtimes.runtime_id group by runtime_name)
	as b on
	a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id;");*/

	$result_runtimes = mysql_query("SELECT runtime_name, runtime_id FROM runtimes");
	$result_devices = mysql_query("SELECT * FROM phones"); //get all phones/devices and their id in the DB

?>

<html>
	<head>

	<link rel="stylesheet" href="style.css" type="tezt/css" />
	<!--Load the AJAX API-->
	<script type="text/javascript" src="https://www.google.com/jsapi"></script>
	<script type="text/javascript">

	xmlhttp=new XMLHttpRequest();

	//function for updating the page content according to form content
	function send_form() {
		//build request string
		var req = "device=";
		req += document.getValueById('device_id');
		req += '&?gl_test=' + document.getValueById('device_id').innerHTML;
		xmlhttp.open("GET", "./index.php?" + req, false);
		xmlhttp.send();
		document.getElementByTagName('html').innerHTML = xmlhttp.responseText;
	}

	// Load the Visualization API and the piechart package.
	google.load('visualization', '1.0', {'packages':['corechart']});

	// Set a callback to run when the Google Visualization API is loaded.
	google.setOnLoadCallback(drawChart);

	// Callback that creates and populates a data table,
	// instantiates the pie chart, passes in the data and
	// draws it.
	function drawChart() {

	// Create the data table for the latest linpack runs.
	var data = new google.visualization.DataTable();
	data.addColumn('string', 'Runtime');
	data.addColumn('number', 'MFLOPS');
	data.addRows([
	<?php
	while ($row = mysql_fetch_assoc($result_latest_runs)) {
		$runtime = $row['runtime_name'];
		$phone = $row['phone_name'];
		$mflops = $row['mflops'];
		$i++;
		if($i < $num_rows)
			echo "['".$runtime." (".$phone.")', $mflops],";
		else
			echo "['".$runtime." (".$phone.")', $mflops]";
		}

	?>
      ]);

	  // Create the data table.
	var data2 = new google.visualization.DataTable();
	data2.addColumn('string', 'Runtime');
	data2.addColumn('number', 'MFLOPS');
	data2.addRows([
	<?php
	$i = 0;
	$num_rows = mysql_num_rows($result_average_runtime);
	while ($row = mysql_fetch_assoc($result_average_runtime)) {
		$runtime = $row['runtime_name'];
		$avgmflops = $row['avgmflops'];
		$i++;
		if($i < $num_rows)
			echo "['".$runtime."', $avgmflops],";
		else
			echo "['".$runtime."', $avgmflops]";
		}

	?>
      ]);

	var data4 = new google.visualization.DataTable();
        data4.addColumn('string', 'Runtime');
        data4.addColumn('number', 'Fill rate (no texture): planes X FPS');
        data4.addColumn('number', 'Fill rate (texture): planes X FPS');
		data4.addColumn('number', 'Dynamic object: #vertices');
		data4.addColumn('number', 'Spinning box: FPS');
        data4.addRows([
        <?php
		$i = 0;
		$num_rows = mysql_num_rows($result_latest_opengl);
		while ($row = mysql_fetch_assoc($result_latest_opengl)) {
			$runtime = $row['runtime_name'];
			$phone = $row['phone_name'];
			$test1 = $row['test1'];
			$test2 = $row['test2'];
			$test3 = $row['test3'];
			$test4 = $row['test4'];
			$i++;
			if($i < $num_rows)
				echo "['".$runtime." (".$phone.")', $test1, $test2, $test3, $test4],";
			else
				echo "['".$runtime." (".$phone.")', $test1, $test2, $test3, $test4]";
		}
		?>
    ]);

	var data5 = new google.visualization.DataTable();
        data5.addColumn('string', 'Runtime');
        data5.addColumn('number', 'Fill rate (no texture): planes X FPS');
        data5.addColumn('number', 'Fill rate (texture): planes X FPS');
		data5.addColumn('number', 'Dynamic object: #vertices');
		data5.addColumn('number', 'Spinning box: FPS');
        data5.addRows([
        <?php
		$i = 0;
		$num_rows = mysql_num_rows($result_average_opengl);
		while ($row = mysql_fetch_assoc($result_average_opengl)) {
			$runtime = $row['runtime_name'];
			$phone = $row['phone_name'];
			$test1 = $row['test1'];
			$test2 = $row['test2'];
			$test3 = $row['test3'];
			$test4 = $row['test4'];
			$i++;
			if($i < $num_rows)
				echo "['".$runtime."', $test1, $test2, $test3, $test4],";
			else
				echo "['".$runtime."', $test1, $test2, $test3, $test4]";
		}
		?>
    ]);

	var hist_data = new google.visualization.DataTable();
	<?php
		echo "hist_data.addColumn('string', 'Days ago');\n";
		$i = 0;
		$runtime_map = array();
		while($row = mysql_fetch_assoc($result_runtimes)) {
			$i++;
			$runtime = $row['runtime_name'];
			echo "hist_data.addColumn('number', '$runtime');\n";
			$runtime_map[$row['runtime_id']] = $i;
		}
		//echo "hist_data.addColumn('number', 'MFLOPS');\n";
		$num_rows = mysql_num_rows($result_runtimes);
		echo "hist_data.addRows($num_rows);\n";

		$row_num = 0;
		$col_num = 0;
		while($row = mysql_fetch_assoc($result_hist)) {
			$avgmflops = $row['avgmflops'];
			$curr_datediff = $row['datediff'];
			$runtime_id = $row['run_id'];
			if($row['datediff'] != $datediff){ //if not matching the prev value
				$row_num++;
				$col_num = 0;
				echo "hist_data.setValue($row_num, 0, '$curr_datediff');\n";
			}
			$col_num++;
			echo "hist_data.setValue($row_num, $runtime_map[$runtime_id], $avgmflops);\n";
			$datediff = $row['datediff'];
		}
    ?>

	var data6 = new google.visualization.DataTable();


		data6.addColumn('string', 'Days ago');
		data6.addColumn('number', 'Fill rate (no texture): planes X FPS');
		data6.addColumn('number', 'Fill rate (texture): planes X FPS');
		data6.addColumn('number', 'Dynamic object: #vertices');
		data6.addColumn('number', 'Spinning box: FPS');

		//echo "data6.addColumn('number', 'MFLOPS');\n";
		data6.addRows([
		<?php
		$i = 0;
		$num_rows = 4;
		$row_num = 0;
		$col_num = 0;
		$datediff = 0; //reset
		$num_rows = mysql_num_rows($result_hist_opengl_all_tests);
		while($row = mysql_fetch_assoc($result_hist_opengl_all_tests)) {
			$test1 = $row['test1'];
			$test2 = $row['test2'];
			$test3 = $row['test3'];
			$test4 = $row['test4'];
			$curr_datediff = $row['datediff'];
			$runtime_id = $row['run_id'];
			$i++;
			if($i < $num_rows){
				echo "['".$curr_datediff."', $test1, $test2, $test3, $test4],";
			}else{
				echo "['".$curr_datediff."', $test1, $test2, $test3, $test4]";
			}
			//echo "data6.setValue($row_num, $runtime_map[$runtime_id], $test1, $test2, $test3, $test4);\n";
			//$datediff = $row['datediff'];
		}
		?>
		]);

		var data7 = new google.visualization.DataTable();
        data7.addColumn('string', 'Runtime');
        data7.addColumn('number', 'alloc str');
		data7.addColumn('number', 'alloc void');
        data7.addColumn('number', 'alloc small obj');
		data7.addColumn('number', 'alloc big obj');
		data7.addColumn('number', 'access array & vector');
		data7.addColumn('number', 'add to vector');
        data7.addColumn('number', 'access small obj');
		data7.addColumn('number', 'access big obj');
        data7.addRows([
        <?php
		$i = 0;
		$num_rows = mysql_num_rows($result_latest_membench);
		while ($row = mysql_fetch_assoc($result_latest_membench)) {
			$runtime = $row['runtime_name'];
			$phone = $row['phone_name'];
			$alloc_str = $row['alloc_str'];
			$alloc_void = $row['alloc_void'];
			$alloc_small_objects = $row['alloc_small_objects'];
			$alloc_big_objects = $row['alloc_big_objects'];
			$access_array_vector = $row['access_array_vector'];
			$add_vector = $row['add_vector'];
			$access_small_objects = $row['access_small_objects'];
			$access_big_objects = $row['access_big_objects'];
			$i++;
			if($i < $num_rows)
				echo "['".$runtime." (".$phone.")', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects],";
			else
				echo "['".$runtime." (".$phone.")', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects]";
		}
		?>
		]);

		var data8 = new google.visualization.DataTable();
        data8.addColumn('string', 'Runtime');
        data8.addColumn('number', 'alloc str');
		data8.addColumn('number', 'alloc void');
        data8.addColumn('number', 'alloc small obj');
		data8.addColumn('number', 'alloc big obj');
		data8.addColumn('number', 'access array & vector');
		data8.addColumn('number', 'add to vector');
        data8.addColumn('number', 'access small obj');
		data8.addColumn('number', 'access big obj');
        data8.addRows([
        <?php
		$i = 0;
		$num_rows = mysql_num_rows($result_average_membench);
		while ($row = mysql_fetch_assoc($result_average_membench)) {
			$runtime = $row['runtime_name'];
			$phone = $row['phone_name'];
			$alloc_str = $row['alloc_str'];
			$alloc_void = $row['alloc_void'];
			$alloc_small_objects = $row['alloc_small_objects'];
			$alloc_big_objects = $row['alloc_big_objects'];
			$access_array_vector = $row['access_array_vector'];
			$add_vector = $row['add_vector'];
			$access_small_objects = $row['access_small_objects'];
			$access_big_objects = $row['access_big_objects'];
			$i++;
			if($i < $num_rows)
				echo "['".$runtime."', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects],";
			else
				echo "['".$runtime."', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects]";
		}
		?>
		]);

		var data9 = new google.visualization.DataTable();
        data9.addColumn('string', 'Days ago');
        data9.addColumn('number', 'alloc str');
		data9.addColumn('number', 'alloc void');
        data9.addColumn('number', 'alloc small obj');
		data9.addColumn('number', 'alloc big obj');
		data9.addColumn('number', 'access array & vector');
		data9.addColumn('number', 'add to vector');
        data9.addColumn('number', 'access small obj');
		data9.addColumn('number', 'access big obj');
        data9.addRows([
        <?php
		$i = 0;
		$num_rows = mysql_num_rows($result_hist_membench);
		while ($row = mysql_fetch_assoc($result_hist_membench)) {
			$runtime = $row['runtime_name'];
			$datediff = $row['datediff'];
			$phone = $row['phone_name'];
			$alloc_str = $row['alloc_str'];
			$alloc_void = $row['alloc_void'];
			$alloc_small_objects = $row['alloc_small_objects'];
			$alloc_big_objects = $row['alloc_big_objects'];
			$access_array_vector = $row['access_array_vector'];
			$add_vector = $row['add_vector'];
			$access_small_objects = $row['access_small_objects'];
			$access_big_objects = $row['access_big_objects'];
			$i++;
			if($i < $num_rows)
				echo "['".$datediff."', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects],";
			else
				echo "['".$datediff."', $alloc_str, $alloc_void, $alloc_small_objects, $alloc_big_objects, $access_array_vector, $add_vector, $access_small_objects, $access_big_objects]";
		}
		?>
		]);

    // Set chart options
    var options = {'title':'Latest LINPACK results',
                     'width':600,
                     'height':400};

	var options2 = {'title':'Average LINPACK results over the last 30 days',
                     'width':600,
                     'height':400};

	var options3 = {'title':'LINPACK history of chosen device, on different runtimes',
					 'width':600,
                     'height':400,
			vAxis: {title: 'MFLOPS'},
			hAxis: {title: 'Days ago'}};

	var options4 = {
          width: 600, height: 400,
          title: 'Latest OpenGLES results'};

	var options5 = {
          width: 600, height: 400,
          title: 'Average OpenGLES results over the last 30 days'};

	var options6 = {'title':'OpenGLES history of chosen device and runtime',
					 width:600,
                     height:400
					 };

	var options7 = {title:'Latest Memory Benchmark results (logarithmic scale)',
                     width:600,
                     height:400,
					 hAxis: {title: 'x 1000 memory operations', minValue: -1000, logScale: true}
					 };

	var options8 = {title:'Average Memory Benchmark results over the last 30 days (logarithmic scale)',
                     width:600,
                     height:400,
					 hAxis: {title: 'x 1000 memory operations', minValue: -1000, logScale: true}
					 };

	var options9 = {title:'Memory Benchmark history on specific device and runtime (logarithmic scale)',
		width:600,
		height:400,
		vAxis: {title: 'x 1000 memory operations', logScale: true},
		hAxis: {title: 'Days ago'}

					 };


      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.BarChart(document.getElementById('chart_div'));
	  var chart2 = new google.visualization.BarChart(document.getElementById('chart_div2'));
	  var chart3 = new google.visualization.LineChart(document.getElementById('chart_div3'));
	  var chart4 = new google.visualization.ColumnChart(document.getElementById('chart_div4'));
	  var chart5 = new google.visualization.ColumnChart(document.getElementById('chart_div5'));
	  var chart6 = new google.visualization.LineChart(document.getElementById('chart_div6'));
	  var chart7 = new google.visualization.BarChart(document.getElementById('chart_div7'));
	  var chart8 = new google.visualization.BarChart(document.getElementById('chart_div8'));
	  var chart9 = new google.visualization.LineChart(document.getElementById('chart_div9'));
      chart.draw(data, options);
	  chart2.draw(data2, options2);
	  chart3.draw(hist_data, options3);
	  chart4.draw(data4, options4);
	  chart5.draw(data5, options5);
	  chart6.draw(data6, options6);
	  chart7.draw(data7, options7);
	  chart8.draw(data8, options8);
	  chart9.draw(data9, options9);

    }

    </script>
  </head>

  <body>

	<!--Div that will hold the header-->
    <div id="header_div"><h2>MoSync Benchmarks</h2>
	<h3>runtime:
	<?php
		$res = mysql_query("SELECT runtime_name FROM runtimes WHERE runtime_id = '$runtime_id'");
		$row = mysql_fetch_assoc($res);
		echo $row['runtime_name'].', device: ';
		$res = mysql_query("SELECT phone_name FROM phones WHERE phone_id = '$device'");
		$row = mysql_fetch_assoc($res);
		echo $row['phone_name'];
	?>
	</h3></div>

	<?php
	$res = mysql_query("SELECT * FROM automatic_runs WHERE result='FAIL'");
	for($i = 0; $row = mysql_fetch_assoc($res); $i++){
		if($i == 0){
			echo '<div id="bench_failed"><p>One or more of the last 10 automatic benchmark suite runs failed, check android_result.out and result.out on the build server for more info.<br />';
		}
		echo 'Run ended at '.$row['timestamp'].' failed!<br />';
	}
	?>
	<p></div>
		<form>
		<select name="run_id">
		<?php
		$i = 0;
		$runtimes = mysql_query("SELECT * FROM runtimes;");
		while($row = mysql_fetch_assoc($runtimes)){
			if($row['runtime_id'] == $runtime_id)
				echo '<option selected="selected" value="'.$row['runtime_id'].'">'.$row['runtime_name'].'</option>';
			else
				echo '<option value="'.$row['runtime_id'].'">'.$row['runtime_name'].'</option>';
		}
		?>
		</select>
		<select name="device">
		<?php
			while($row = mysql_fetch_assoc($result_devices)){
				if($row['phone_id'] == $device)
					echo '<option id="phone_id" selected="selected" value="'.$row['phone_id'].'">'.$row['phone_name'].'</option>';
				else
					echo '<option id="phone_id" value="'.$row['phone_id'].'">'.$row['phone_name'].'</option>';
			}
		?>
		</select>
		<input type="submit" value="Update charts" onClick="send_form()">
	</form>

    <!--Div that will hold the pie chart-->
    <div id="chart_div" class="chart"></div>
	<div id="chart_div2" class="chart"></div>
	<div id="chart_div3" class="chart"></div>
	<div id="chart_div4" class="chart"></div>
	<div id="chart_div5" class="chart"></div>
	<div id="chart_div6" class="chart"></div>
	<div id="chart_div7" class="chart"></div>
	<div id="chart_div8" class="chart"></div>
	<div id="chart_div9" class="chart"></div>


  </body>
</html>

<?php //free result and close db connection
	mysql_free_result($result);
	require_once('db_conn_close.php');
?>
