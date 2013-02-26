First step: Set the  DEVELOPER_PUBLIC_KEY token, taken from the DEVELOPER CONSOLE.

The Button "Restore previous purchases" is useful when  you have logged in on another device and you wish all your products to be restored.
If there is no product to be restored, pressing the button will have no effect whatsoever.

First section of the main screen displays  "Items you have previously purchased" Those will be items you already purchased on that particular device,
and were stored to the database. NOTE: Each time the application is re-installed, application's data needs also to be cleared (from Settings).

The second section displays the items available for purchase.

The last section is the history section: "Items you own" that is updated during the application's execution.
By pressing on an owned product here, the user will be presented a dialog with receipt details. NOTE that for the test product: android.test.purchased
the receipt will not hold valid data, so the transactiondate=0

<< The In-app Billing Version 3 service only supports managed in-app products at this point>>
NOTE: there are 2 product types:
     - MANAGED products that can be bought multiple times. ( Like a FUEL product for a gaming app). Consumable items cannot be restored.
      When consumable products are purchased, they are automatically consumed, therefore the user may purchase them again.
     - UNMANAGED products can only be bought once per app, and the server does not store any associated transaction information.
       Those types of products are not handled in this example.


More info about in-app products can be found here: http://developer.android.com/training/in-app-billing/list-iab-products.html