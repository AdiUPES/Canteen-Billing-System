# Campus Canteen Billing System

A Windows console application written in C. It takes canteen orders, calculates GST and discounts, prints a red tax invoice for each customer, and shows an end-of-day sales report.

Built as a B.Tech first-semester project using only arrays, loops, conditions, and functions.

## Features

- 18 menu items shown in a three-column layout.
- Add items and quantities to an order.
- Remove some or all of an item before billing.
- View the current order before billing.
- Dine-in and takeaway options (packing charge applies to takeaway).
- Slab-based discount, CGST and SGST, and rounding to the nearest rupee.
- Unique bill IDs starting from `CNT-1001`.
- Red bill text and green application text.
- Thank-you message at the end of every bill.
- End-of-day report shown only when the day is closed.

## Menu

| No. | Item | Price (Rs.) |
|---|---|---:|
| 1 | Samosa | 50 |
| 2 | Burger | 80 |
| 3 | Pizza | 100 |
| 4 | Tea | 30 |
| 5 | Sandwich | 40 |
| 6 | Coffee | 40 |
| 7 | Cold Coffee | 70 |
| 8 | Maggi | 60 |
| 9 | Veg Momos | 70 |
| 10 | Paneer Roll | 90 |
| 11 | French Fries | 70 |
| 12 | Chole Bhature | 90 |
| 13 | Veg Thali | 120 |
| 14 | Masala Dosa | 80 |
| 15 | Pav Bhaji | 90 |
| 16 | Spring Roll | 60 |
| 17 | Lassi | 50 |
| 18 | Cold Drink | 40 |

## Requirements

- Windows (the code uses `windows.h`, `cls`, `color`, and `title`).
- MinGW GCC or another C compiler that supports `windows.h`.

## Project Structure

```text
canteen-billing-system/
├── canteen.c
└── README.md
```

## Build and Run

Compile:

```bash
gcc -Wall -Wextra canteen.c -o CanteenBilling.exe -static
```

Run:

```powershell
.\CanteenBilling.exe
```

Or double-click `CanteenBilling.exe`. The `-static` flag bundles everything into one file, so it runs on other Windows PCs without extra DLLs.

## How to Use

1. Choose **1. New Order** from the main menu.
2. Select **1. Dine-in** or **2. Takeaway**.
3. Choose **1. Add Item**, enter the menu number, then the quantity.
4. Repeat to add more items. Use **2. Remove Item** to fix mistakes and **3. View Order** to check the order.
5. Choose **4. Generate Bill** to print the bill.
6. Press Enter to clear the screen for the next customer.
7. Choose **2. Close Day & Exit** to see the end-of-day report.

Choosing **5. Cancel Order** discards the current order without recording a sale.

## Code Overview

| Function | Purpose |
|---|---|
| `setColor()` | Changes the console text colour |
| `printLine()` | Prints a row of a repeated character (for borders) |
| `clearInput()` | Removes leftover characters from the input buffer |
| `readInt()` | Reads an integer and asks again if the input is not a number |
| `pressEnter()` | Pauses until the user presses Enter |
| `showMenu()` | Prints the menu in three columns |
| `countItems()` | Returns the total number of items in the current order |
| `showCart()` | Displays the current order |
| `addItem()` | Adds an item and quantity to the order |
| `removeItem()` | Removes some or all of an item from the order |
| `generateBill()` | Calculates totals, prints the bill, and records the sale |
| `endOfDayReport()` | Prints the summary and item-wise sales |
| `newOrder()` | Handles one customer's order from start to bill |
| `main()` | Shows the main menu and sets up the window |

### Main Data Arrays

| Name | Type | Stores |
|---|---|---|
| `items[N][20]` | `char` | Item names |
| `prices[N]` | `int` | Price of each item |
| `cart[N]` | `int` | Quantities in the current order |
| `soldToday[N]` | `int` | Quantities sold in this session |

## Billing Rules

### Discount

| Subtotal | Discount |
|---|---:|
| Below Rs.500 | 0% |
| Rs.500 to below Rs.1000 | 5% |
| Rs.1000 and above | 10% |

### Charges

- **Packing:** Rs.5 per unit, takeaway only.
- **CGST:** 2.5% of the taxable amount.
- **SGST:** 2.5% of the taxable amount.
- No service charge is included.

These rates are demonstration values. Check current tax rules before using the app for a real canteen.

### Calculation

```text
Subtotal       = Sum of (Quantity × Unit Price)
Discount       = Subtotal × Discount % / 100
Packing Charge = Total Quantity × Rs.5 (takeaway only)
Taxable Amount = Subtotal - Discount + Packing Charge
CGST           = Taxable Amount × 2.5 / 100
SGST           = Taxable Amount × 2.5 / 100
Total          = Taxable Amount + CGST + SGST
Grand Total    = Total rounded to the nearest rupee
Round Off      = Grand Total - Total
```

### Example

Takeaway order: 2 Burgers, 2 Teas, 3 Veg Thalis.

| Description | Amount (Rs.) |
|---|---:|
| Subtotal | 580.00 |
| Discount (5%) | -29.00 |
| Packing (7 units) | 35.00 |
| Taxable amount | 586.00 |
| CGST (2.5%) | 14.65 |
| SGST (2.5%) | 14.65 |
| Total before rounding | 615.30 |
| Round off | -0.30 |
| **Grand total** | **615.00** |

## Concepts Used

- One-dimensional arrays (prices, cart, sales count).
- Two-dimensional character arrays (item names).
- Functions to split the program into tasks.
- `for`, `while`, and `do-while` loops.
- `if-else` and `switch` statements.
- Formatted input and output with `printf` and `scanf`.

## Limitations

- All data is kept in memory, so it is lost when the program closes.
- Bill IDs restart at `CNT-1001` on every run.
- "Today's sales" means the current run, not a calendar day.
- If two items tie for the most sales, the first one in menu order is shown.
- The 50-unit limit applies to each addition, not to the total in the cart.
- `float` is used for money, which can cause small rounding differences.
- The GSTIN on the bill is a placeholder.
- Output is shown on screen only. Bills are not saved or printed.
- Red text applies to the console only, not to paper printouts.

## Future Improvements

- Save bills and reports to files.
- Keep bill numbers across runs.
- Add date and time to bills.
- Handle ties for the best-selling item.
- Use integer paise instead of `float` for money.
- Add printer support.
