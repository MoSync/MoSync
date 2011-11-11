<?php
	require_once('db_conn.php'); //set up the connection to the database
	
	/* this SQL statement returns the latest testruns of the linpack benchmark, the latest per distinct runtime */
	$result = mysql_query("SELECT
    runtime_name, mflops
	FROM
    linpack_testruns a
    inner join 
        (SELECT runtime_name, max(id) as maxid FROM linpack_testruns, runtimes WHERE linpack_testruns.runtime_id = runtimes.runtime_id group by runtime_name) as b on
        a.id = b.maxid");
	$num_rows = mysql_num_rows($result);
	$i = 0;

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

	// Create the data table.
	var data = new google.visualization.DataTable();
	data.addColumn('string', 'Runtime');
	data.addColumn('number', 'MFLOPS');
	data.addRows([
	<?php
	while ($row = mysql_fetch_assoc($result)) {
		$runtime = $row['runtime_name'];
		$mflops = $row['mflops'];
		$i++;
		if($i < $num_rows)
			echo "['".$runtime."', $mflops],";
		else
			echo "['".$runtime."', $mflops]";
		}
		
	?>
      ]);

      // Set chart options
      var options = {'title':'Latest LINPACK Benchmark runs',
                     'width':600,
                     'height':400};

      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.BarChart(document.getElementById('chart_div'));
      chart.draw(data, options);
    }
    </script>
  </head>

  <body>
  
	<!--Div that will hold the header-->
    <div id="header_div"><h1>MoSync Benchmarks</h1></div>
  
    <!--Div that will hold the pie chart-->
    <div id="chart_div"></div>
  </body>
</html>

<?php //free result and close db connection
	mysql_free_result($result);
	require_once('db_conn_close.php');
?>