/**
 * Created by airing on 16/3/10.
 */

function js_getMapRoute() {
    var route = document.getElementById("map_route");
    js_getPosition(route);
}

function js_getSpeedRoute() {
	var route = document.getElementById("speed_route");
    js_getSpeed(route);
}

function js_getTemperatureRoute() {
	var route = document.getElementById("temperature_route");
    js_getSpeed(route);
}

function js_getHumidityRoute() {
	var route = document.getElementById("humidity_route");
    js_getSpeed(route);
}


function js_getPosition(route) {
    $.ajax({
        type: "POST",
        url: "http://localhost:8080/emps/get_position.action",
        data: {carRoute: route.value},
        success: function (data) {
            var jsondata = eval(data);
            var status = jsondata.status;
            var positions = jsondata.positions;
            var x = [];
            var y = [];
        	var x_max = 1000;
        	var y_max = 500;
        	var x_scale = 1;
        	var y_scale = 1;
        	
            for(var i = 0; i < positions.length; i++) {
            	if(Math.abs(positions[i].carX) > x_max)
            		x_max = positions[i].carX;
            	if(Math.abs(positions[i].carY) > y_max) 
            		y_max = positions[i].carY;
            }
            
            x_scale = x_max / 500;
            y_scale = y_max / 250;
            
            for(var i = 0; i < positions.length; i++) {
            	x.push(positions[i].carX / x_scale);
            	y.push(-positions[i].carY / y_scale);
            }
            drawMap(x, y);
        },
        error: function (XMLHttpRequest, textStatus) {
            alert("XMLHttpRequest.status" + XMLHttpRequest.status);
        }
    });

}

function drawMap(x, y) {
    var canvas = document.getElementById("canvas");
    canvas.width = 1000;
    canvas.height = 500;
    var context = canvas.getContext("2d");
    context.moveTo(canvas.width/2, canvas.height/2);
	for (var i = 0; i < x.length; i++) {
	    context.lineTo(canvas.width/2 + x[i], canvas.height/2 + y[i]);
	}
    context.lineWidth = 3;
    context.strokeStyle = "#AA394C";
    context.stroke();
}

function js_getSpeed(route) {
	$.ajax({
        type: "POST",
        url: "http://localhost:8080/emps/get_position.action",
        data: {carRoute: route.value},
        success: function (data) {
            var jsondata = eval(data);
            var status = jsondata.status;
            var positions = jsondata.positions;
            var dataset = [];
            for(var i = 0; i < positions.length; i++) {
            	dataset.push(positions[i].carSpeed)
            }
            showDataSet(dataset, "#svg1");
        },
        error: function (XMLHttpRequest, textStatus) {
            alert("XMLHttpRequest.status" + XMLHttpRequest.status);
        }
    });
}

function js_getTemperature(route) {
	$.ajax({
        type: "POST",
        url: "http://localhost:8080/emps/get_position.action",
        data: {carRoute: route.value},
        success: function (data) {
            var jsondata = eval(data);
            var status = jsondata.status;
            var positions = jsondata.positions;
            var dataset = [];
            for(var i = 0; i < positions.length; i++) {
            	dataset.push(positions[i].carSpeed)
            }
            showDataSet(dataset, "#svg2");
        },
        error: function (XMLHttpRequest, textStatus) {
            alert("XMLHttpRequest.status" + XMLHttpRequest.status);
        }
    });
}

function js_getHumidity(route) {
	$.ajax({
        type: "POST",
        url: "http://localhost:8080/emps/get_position.action",
        data: {carRoute: route.value},
        success: function (data) {
            var jsondata = eval(data);
            var status = jsondata.status;
            var positions = jsondata.positions;
            var dataset = [];
            for(var i = 0; i < positions.length; i++) {
            	dataset.push(positions[i].carSpeed)
            }
            showDataSet(dataset, "#svg3");
        },
        error: function (XMLHttpRequest, textStatus) {
            alert("XMLHttpRequest.status" + XMLHttpRequest.status);
        }
    });
}

function showDataSet(dataset, id) {
    //Width and height
    var w = 1000;
    var h = 500;

    var xScale = d3.scale.ordinal()
            .domain(d3.range(dataset.length))
            .rangeRoundBands([0, w], 0.05);

    var yScale = d3.scale.linear()
            .domain([0, d3.max(dataset)])
            .range([0, h]);

    //Create SVG element
    var svg = d3.select(id)
            .attr("width", w)
            .attr("height", h);

    //Create bars
    svg.selectAll("rect")
            .data(dataset)
            .enter()
            .append("rect")
            .attr("x", function(d, i) {
                return xScale(i);
            })
            .attr("y", function(d) {
                return h - yScale(d);
            })
            .attr("width", xScale.rangeBand())
            .attr("height", function(d) {
                return yScale(d);
            })
            .attr("fill", function(d) {
                //return "rgb(0, 0, " + (d * 10) + ")";
            	return "rgb(67, 170, 170)";
            })
            .on("mouseover", function(d) {
                d3.select(this)
                        .attr("fill", "orange");
                var xPosition = parseFloat(d3.select(this).attr("x")) + xScale.rangeBand() / 2;
                var yPosition = parseFloat(d3.select(this).attr("y")) / 2 + h / 2;

                //Update the tooltip position and value
                d3.select("#tooltip")
                        .style("left", xPosition + "px")
                        .style("top", yPosition + "px")
                        .select("#value")
                        .text(d);

                //Show the tooltip
                d3.select("#tooltip").classed("hidden", false);
            })
            .on("mouseout", function(d) {
                d3.select(this)
                        //.attr("fill", "rgb(0, 0, " + (d * 10) + ")");
                .attr("fill", "rgb(67, 170, 170)");

            });

    //Create labels
    svg.selectAll("text")
            .data(dataset)
            .enter()
            .append("text")
            .text(function(d) {
                return d;
            })
            .attr("text-anchor", "middle")
            .attr("x", function(d, i) {
                return xScale(i) + xScale.rangeBand() / 2;
            })
            .attr("y", function(d) {
                return h - yScale(d) + 14;
            })
            .attr("font-family", "sans-serif")
            .attr("font-size", "11px")
            .attr("fill", "white");
}