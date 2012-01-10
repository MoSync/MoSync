<?php
	/*
	TODO Show the performance trend of a runtime on different benchmarks
	*/

	require_once('db_priv_conn.php'); //set up the connection to the database
	
	/* store the input retrieved via HTTP GET */
	$device = mysql_real_escape_string($_GET['device']); 				//the device we want extra information about
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
	$result_latest_membench = mysql_query("SELECT  *
	FROM
    membench_testruns a
    inner join 
        (SELECT runtime_name, max(id) as maxid FROM membench_testruns, runtimes WHERE membench_testruns.runtime_id = runtimes.runtime_id group by runtime_name) as b on
        a.id = b.maxid JOIN phones ON a.phone_id=phones.phone_id");
	
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
	FROM opengl_testruns, runtimes  WHERE runtimes.runtime_id = opengl_testruns.runtime_id AND phone_id = '$device'  
	GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC;");
	
	$result_runtimes = mysql_query("SELECT runtime_name FROM runtimes");
	$result_devices = mysql_query("SELECT * FROM phones"); //get all phones/devices and their id in the DB

?>

<html>
	<head>
	
	<link rel="stylesheet" href="style.css" type="tezt/css" />
	<!--Load the AJAX API-->
	<script type="text/javascript" src="https://www.google.com/jsapi"></script>
	<script type="text/javascript">
    
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
		while($row = mysql_fetch_assoc($result_runtimes)) {
			$runtime = $row['runtime_name'];
			echo "hist_data.addColumn('number', '$runtime');\n";
		}
		//echo "hist_data.addColumn('number', 'MFLOPS');\n";
		echo "hist_data.addRows(5);\n";
		
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
			echo "hist_data.setValue($row_num, $runtime_id, $avgmflops);\n";
			$datediff = $row['datediff'];
		}
    ?>
	
	var data6 = new google.visualization.DataTable();
	<?php
		$result_runtimes = mysql_query("SELECT runtime_name FROM runtimes"); //we need this again, yes we could have saved it and saved a db-query...
		echo "data6.addColumn('string', 'Days ago');\n";
		while($row = mysql_fetch_assoc($result_runtimes)) {
			$runtime = $row['runtime_name'];
			echo "data6.addColumn('number', '$runtime');\n";
		}
		//echo "data6.addColumn('number', 'MFLOPS');\n";
		echo "data6.addRows(5);\n";
		
		$row_num = 0;
		$col_num = 0;
		while($row = mysql_fetch_assoc($result_hist_opengl)) {
			$test = $row['test'];
			$curr_datediff = $row['datediff'];
			$runtime_id = $row['run_id'];
			if($row['datediff'] != $datediff){ //if not matching the prev value
				$row_num++;
				$col_num = 0;
				echo "data6.setValue($row_num, 0, '$curr_datediff');\n";
			}
			$col_num++;
			echo "data6.setValue($row_num, $runtime_id, $test);\n";
			$datediff = $row['datediff'];
		}
    ?>
	
    // Set chart options
    var options = {'title':'Latest LINPACK results',
                     'width':500,
                     'height':400};
					 
	var options2 = {'title':'Average LINPACK results over the last 30 days',
                     'width':500,
                     'height':400};
					 
	var options3 = {'title':'LINPACK history on specific device, different runtimes over the last 30 days(MFLOPS)',
					 'width':500,
                     'height':400};
					 
	var options4 = {
          width: 500, height: 400,
          title: 'Latest OpenGLES results'};
		
	var options5 = {
          width: 500, height: 400,
          title: 'Average OpenGLES results over the last 30 days'};
		  
	var options6 = {'title':'OpenGLES history on specific device and test, different runtimes over the last 30 days',
					 'width':500,
                     'height':400};

      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.BarChart(document.getElementById('chart_div'));
	  var chart2 = new google.visualization.BarChart(document.getElementById('chart_div2'));
	  var chart3 = new google.visualization.LineChart(document.getElementById('chart_div3'));
	  var chart4 = new google.visualization.ColumnChart(document.getElementById('chart_div4'));
	  var chart5 = new google.visualization.ColumnChart(document.getElementById('chart_div5')); 
	  var chart6 = new google.visualization.LineChart(document.getElementById('chart_div6'));
      chart.draw(data, options);
	  chart2.draw(data2, options2);
	  chart3.draw(hist_data, options3);
	  chart4.draw(data4, options4);
	  chart5.draw(data5, options5);
	  chart6.draw(data6, options6);
    }
	
	
    </script>
  </head>

  <body>
  
	<!--Div that will hold the header-->
    <div id="header_div"><h1>MoSync Benchmarks</h1></div>
  
    <!--Div that will hold the pie chart-->
    <div id="chart_div" class="chart"></div>
	<div id="chart_div2" class="chart"></div>
	<div id="chart_div3" class="chart"></div>
	<div id="chart_div4" class="chart"></div>
	<div id="chart_div5" class="chart"></div>
	<div id="chart_div6" class="chart"></div>
	<form action="<?php $_SERVER['SCRIPT_FILENAME']?>" method="get">
		<select name="device">
		<?php 
			while($row = mysql_fetch_assoc($result_devices)) {
				echo '<option value="'.$row['phone_id'].'">'.$row['phone_name'].'</option>';
			}
		?>
		</select>
		<select name="gl_test">
			<option value="1">Fill rate (no texture)</option>
			<option value="2">Fill rate (texture)</option>
			<option value="3">Dynamic object</option>
			<option value="4">Spinning box</option>
		</select>
		<input type="submit" value="Update charts">
	</form>
	
  </body>
</html>

<?php //free result and close db connection
	mysql_free_result($result);
	require_once('db_conn_close.php');
?>