/*
* @file app.ui.js
* @author Mikael Kindborg
*
* This file contains UI code at a more abstract level.
* The intention is to make it easier to switch UI libraries
* without having to update/rewrite all the UI code.
*/

app.ui = (function()
{
	var ui = {};

	// Make sure we get the correct event type on every touch system.

	ui.showMessage = function(message)
	{
		// Windows Phone does not support alert, show the message
		// in the user name field for now.
		if (mosync.isWindowsPhone)
		{
			ui.setUserNameField(message);
		}
		else
		{
			alert(message);
		}
	};

	ui.getUserNameField = function()
	{
		return $("#userNameField").val();
	};

	ui.setUserNameField = function(name)
	{
		return $("#userNameField").val(name);
	};

	/**
	 * Show the tweets for the user in the input form.
	 */
	ui.showTweets = function()
	{
		// Get the user name from the input field.
		var user = app.ui.getUserNameField();
		if (user.length === 0)
		{
			ui.showMessage("ui.showTweets: Invalid username");
		}
		else
		{
			ui.showTweetsForUser(user);
		}
	};

	/**
	 * Outputs the received tweets to the user-interface.
	 * @param tweets The tweets as an array. Each element
	 * represents one tweet and can be accessed via tweets[i].text
	 */
	ui.showTweetsForUser = function(user)
	{
		// Go to second screen.
		ui.showPage("tweetList");
		ui.showTweetListUserName(user);
		ui.showLoadingIndicator();

		// Get tweets.
		app.getTweetsForUser(user, function(tweets)
		{
			ui.showTweetList(tweets);
		});
	};

	/**
	 * Read and display list of favourite users.
	 */
	ui.readAndDisplayFavouriteUsers = function()
	{
		app.readFavouriteUsers(function(userData, path)
		{
			if (userData)
			{
				ui.displayFavouriteUsers(userData);
			}
		});
	};

	/**
	 * Add a user to the favourite list.
	 */
	ui.addFavouriteUser = function()
	{
		// Get the username from the input field
		var userName = app.ui.getUserNameField();
		if (userName.length === 0)
		{
			ui.showMessage("Invalid username")
			ui.showPage("home");
			return;
		}

		app.addFavouriteUser(userName, function(userList)
		{
			if (!userList)
			{
				app.ui.showMessage("Could not save user");
			}
			else
			{
				ui.displayFavouriteUsers(userList);
				ui.showMessage("User successfully saved");
			}
		});
	};

	/**
	 * Keydown event listener. Gets called when the user presses
	 * a button on his/her keyboard in the input field.
	 * @param keynum The pressed key (as an ASCII value).
	 */
	ui.keydown = function(keynum)
	{
		if (keynum == 13)
		{
			// TODO: Perhaps we need to use a timer to
			// fix a virtual keyboard bug on Android 4.
			//setTimeout(function () {
				ui.showTweets();
			//}, 1);
		}
	};

	/**
	 * Set up event bindings and initialize the UI.
	 */
	ui.initialize = function()
	{
		// Handle the back key event (on Android).
		document.addEventListener(
			"backbutton",
			function()
			{
				ui.goBack();
			},
			true);

		ui.initializeFramework();

		ui.readAndDisplayFavouriteUsers();
	};

	/**
	 * Diplay the list of favourite users in the UI.
	 * @param userData Array of user names.
	 */
	ui.displayFavouriteUsers = function(userData)
	{
		alert("ui.displayFavouriteUsers: Implement for the specific UI framework used");
	};

	/**
	 * Displays the Twitter user name in the UI.
	 * @param user The Twitter user name.
	 */
	ui.showTweetListUserName = function(user)
	{
		//$("#pageTitle").html(user);
		alert("ui.showTweetListUserName: Implement for the specific UI framework used");
	};

	/**
	 * Displays a loading indicator in the UI.
	 */
	ui.showLoadingIndicator = function()
	{
		//$("#tweetList").html("<li>Loading...</li>");
		alert("ui.showLoadingIndicator: Implement for the specific UI framework used");
	};

	/**
	 * Outputs the list of tweets to the user-interface.
	 * @param tweets The tweets as an array. Each element
	 * represents one tweet and can be accessed via tweets[i].text
	 */
	ui.showTweetList = function(tweets)
	{
		alert("ui.showTweetList: Implement for the specific UI framework used");
	};

	/**
	 * Called by framework when document has loaded.
	 */
	ui.initializeFramework = function()
	{
		alert("ui.initializeFramework: Implement for the specific UI framework used");
	};

	/**
	 * Navigate back one page.
	 */
	ui.goBack = function()
	{
		alert("ui.goBack: Implement for the specific UI framework used");
	};

	/**
	 * Show one page.
	 */
	ui.showPage = function(page)
	{
		alert("ui.showPage: Implement for the specific UI framework used");
	};

	return ui;
})();

// Call app.ui.initialize when document has loaded.
document.addEventListener(
	"DOMContentLoaded",
	app.ui.initialize,
	false);
