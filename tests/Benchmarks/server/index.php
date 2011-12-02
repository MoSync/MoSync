<?php
	require_once('db_priv_conn.php'); //set up the connection to the database
	
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
	
	// get the average result of each runtime
	$result_average_runtime = mysql_query("SELECT runtime_name, AVG( mflops ) AS avgmflops
		FROM linpack_testruns, runtimes
		WHERE runtimes.runtime_id = linpack_testruns.runtime_id
		AND DATEDIFF( NOW( ) , TIMESTAMP ) <30
		GROUP BY runtime_name ORDER BY avgmflops DESC");
		
	// get historical result for a specific device TODO hardcoded device id ATM
	$result_hist = mysql_query("SELECT runtime_name, runtimes.runtime_id as run_id, avg(mflops) as avgmflops, DATEDIFF(now(), timestamp) AS datediff 
	FROM linpack_testruns, runtimes 
	WHERE runtimes.runtime_id = linpack_testruns.runtime_id AND phone_id = 3 
	GROUP BY datediff, runtimes.runtime_id ORDER BY datediff DESC");
	
	$result_runtimes = mysql_query("SELECT runtime_name FROM runtimes");

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
	
      // Set chart options
      var options = {'title':'Latest LINPACK results',
                     'width':500,
                     'height':400};
					 
	var options2 = {'title':'Average LINPACK results over the last 30 days',
                     'width':500,
                     'height':400};
					 
	var options3 = {'title':'LINPACK history on runtimes over the last 30 days(MFLOPS)',
					 'width':500,
                     'height':400};

      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.BarChart(document.getElementById('chart_div'));
	  var chart2 = new google.visualization.BarChart(document.getElementById('chart_div2'));
	  var chart3 = new google.visualization.LineChart(document.getElementById('chart_div3'));
      chart.draw(data, options);
	  chart2.draw(data2, options2);
	  chart3.draw(hist_data, options3);
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
  </body>
</html>

<?php //free result and close db connection
	mysql_free_result($result);
	require_once('db_conn_close.php');
?>