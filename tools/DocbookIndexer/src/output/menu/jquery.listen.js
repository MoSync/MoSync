/**
 * jQuery.Listen - Unattached event handling.
 * Copyright (c) 2007 Ariel Flesler - aflesler(at)gmail(dot)com
 * Licensed under LGPL license (http://www.gnu.org/licenses/lgpl.html).
 * Date: 12/10/2007
 *
 * @projectDescription unattached event handling, using event delegation.
 * @see http://icant.co.uk/sandbox/eventdelegation/
 * Compatible with jQuery 1.2.1, tested on Firefox 2.0.0.6, and IE 6, both on Windows.
 *
 * @author Ariel Flesler
 * @version 0.8
 *
 * @id jQuery.listen
 * @param {String} event Name of the event to listen (f.e: click, mouseover, etc).
 * @param {DOM Element} listener optional: The DOM element to listen from, the document element by default.
 * @param {String|Boolean} selector A simple selector in one of this formats: "#id", "tagname", or "tagname.class".
 * @param {Function} handler The event handler to register.
 *
 * Notes:
 *		-The selectors support is low and will be, likely, increased in further versions.
 *		 Still I consider these 4 options, the most useful and I believe they are enough for many cases.
 *		-Events that don't bubble (like focus and blur), normally, can't be handled using this technique.
 *		-To handle custom events (and focus, blur, etc) check "http://jquery.com/plugins/project/Bubble"
 *		-To stop listening for an event with an element, call jQuery.fn.listen(or jQuery.listen)
 *			with no selector(or false). Instead use true if you want to start it again.
 **/
(function( $ ){
	
	$.listen = function( event, listener, selector, handler ){
		if( $.listen.illegal( event ) )
			throw 'jQuery.Listen > "'+event+'" can\'t be handled because it doesn\'t bubble';
			
		if( typeof listener != 'object' ) //allow some overloading.
			handler = selector, selector = listener, listener = document;
			
		var indexer = jQuery.data(listener,'indexer-'+event) //retrieve the indexer
				   || jQuery.data(listener,'indexer-'+event, new Indexer(event,listener));
		
		switch( selector ){
			case undefined:
			case false: indexer.stop(); break;//stop listening
			default: indexer.append( selector, handler );// register the handler.
			case true: indexer.start(); break;//make it start
		}
	};
	
	$.fn.listen = function( event, selector, handler ){//listen using the prototype
		return this.each(function(){
			$.listen( event, this, selector, handler );						  
		});
	};	
	
	$.extend( $.listen, {
		strict:true, //check for incompatible events ?
		bubbles: {},//this events won't work with this technique
		illegal:function( event ){//is an event incompatible?
			return this.strict && !this.bubbles[event];
		}
	});
	
	/* 'change' should bubble, but IE... :( */
	$.each(('click,dblclick,mousedown,mouseup,mousemove,mouseover,mouseout,'
		   +'keydown,keypress,keyup').split(','), function(i,e){	
		$.listen.bubbles[e] = true;
	});
	
	function Indexer( event, listener ){
		this.ids = {};
		this.names = {};
		this.listener = listener;
		this.event = event;
		Indexer.instances.push( this );//for cleaning up later
	};
	Indexer.instances = [ ];

	Indexer.prototype = {
		constructor: Indexer,
		handler:function( e ){
			var indexer = e.data;
			e.data = null;//don't expose it
			indexer.parse.apply( indexer, arguments );			
		},
		running:false,
		start:function(){//start listening (bind)
			if( this.running ) return; //avoid duplicates
			$.event.add( this.listener, this.event, this.handler, this );
			this.running = true;
		},
		stop:function(){//start listening (unbind)
			if( !this.running ) return;
			$.event.remove( this.listener, this.event, this.handler );
			this.running = false;
		},
		parse:function( e ){
			var node = e.target,
				handlers = [ ];
			if( node.id && this.ids[node.id] )//if this node has an id and there're handlers registered to it..
				push( handlers, this.ids[node.id] );
			
			each([ node.nodeName, '*' ], function(name){//look for handlers registered by name.class.
				var classes = this.names[name];
				if( classes )
					each( push(node.className.split(' '),['*']), function(css){
						if( classes[css] ) push( handlers, classes[css] );//append the handlers to the list.
					});
			},this);
			this.execute( node, handlers, arguments );
			node = handlers = e = null;//cleanup
		},
		append:function( selector, handler ){
			var index = new Index( selector );
			if( index.id ){//we have an id, register the handler to it.
				(this.ids[index.id] || (this.ids[index.id] = [ ])).push( handler );
			}else if( index.nodeName ){//we have an name and/or class
				var name = this.names[index.nodeName] || (this.names[index.nodeName] = { });
				(name[index.className] || (name[index.className] = [ ])).push( handler );
			}else{
				throw 'jQuery.Listen > "' + selector + '" was not recognized as a valid selector.';
			}
		},
		execute:function( node, handlers, args ){
			if( handlers.length )
				each( handlers, function( handler ){
					handler.apply( node, args );
				});
		}
	};
	
	function Index( selector ){//break the selector.
		var match = Index.regex.exec( selector ) || [ ];
		if( match[1] ){
			this.id = match[1];
		}else if( match[2] || match[3] ){
			this.nodeName = match[2] ? match[2].toUpperCase() : '*';
			this.className = match[3] ? match[3].substring(1) : '*';
		}
	};
	Index.regex = /#([\w\d_-]+)$|(\w*)(\.[\w_]+)?$/;//matches "#id", "tagname", ".class" or "tagname.class".
		
	function each( arr, fn, scope ){
		for(var i=0, l=arr.length; i < l; i++ )
			fn.call( scope, arr[i], i );
	};
	function push( arr, arr2 ){
		Array.prototype.push.apply( arr, arr2 );
		return arr;
	};
	
	$.event.add( window, 'unload', function(){// cleanup
		if(Indexer) 
			$.each( Indexer.instances, function(i,idx){
				idx.stop(); idx.listener = null;
			});
	});
	
})( jQuery );