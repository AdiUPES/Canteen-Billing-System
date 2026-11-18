#include <stdio.h>
#include <stdlib.h>       /* for system() - title, colour, clear screen */
#include <windows.h>      /* for changing text colour                   */

#define N 18              /* number of items on the menu          */
#define CGST_RATE 2.5     /* Central GST %                         */
#define SGST_RATE 2.5     /* State GST %   (total GST = 5%)        */
#define PACK_CHARGE 5     /* packing charge per item for takeaway  */
#define MAX_QTY 50        /* max quantity allowed in one entry     */

/* ---------------- MENU ---------------- */
char items[N][20] = {
    "Samosa", "Burger", "Pizza", "Tea", "Sandwich",
    "Coffee", "Cold Coffee", "Maggi", "Veg Momos", "Paneer Roll",
    "French Fries", "Chole Bhature", "Veg Thali", "Masala Dosa", "Pav Bhaji",
    "Spring Roll", "Lassi", "Cold Drink"
};

int prices[N] = {
    50, 80, 100, 30, 40,
    40, 70, 60, 70, 90,
    70, 90, 120, 80, 90,
    60, 50, 40
};

/* ------- CURRENT ORDER AND TODAY'S SALES ------- */
int cart[N];              /* quantity of each item in current order */
int soldToday[N];         /* quantity of each item sold today       */
int billsToday = 0;
int itemsSoldToday = 0;
float salesToday = 0;
int nextBillNo = 1001;    /* bill IDs start from CNT-1001           */

/* Changes the colour of the text printed after this call
   10 = Light Green, 12 = Light Red, 14 = Yellow, 15 = White */
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* Prints a line of the given character */
void printLine(char ch, int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("%c", ch);
    printf("\n");
}

/* Removes extra characters left in the input */
void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Reads a number safely (asks again if user types letters) */
int readInt(void) {
    int n;
    while (scanf("%d", &n) != 1) {
        clearInput();
        printf("Please enter a number: ");
    }
    clearInput();
    return n;
}

/* Waits until the user presses Enter */
void pressEnter(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

/* Shows the menu in 3 columns (horizontal layout) */
void showMenu(void) {
    int i;
    printf("\n");
    printLine('=', 81);
    printf("%46s\n", "CANTEEN MENU");
    printLine('=', 81);

    for (i = 0; i < N; i++) {
        printf("%2d. %-13s Rs.%-4d", i + 1, items[i], prices[i]);
        if ((i + 1) % 3 == 0)
            printf("\n");          /* new row after every 3 items */
        else
            printf(" | ");
    }
    if (N % 3 != 0)
        printf("\n");
    printLine('=', 81);
}

/* Returns total number of items in current order */
int countItems(void) {
    int i, total = 0;
    for (i = 0; i < N; i++)
        total += cart[i];
    return total;
}

/* Shows the current order */
void showCart(void) {
    int i;
    if (countItems() == 0) {
        printf("\nYour order is empty.\n");
        return;
    }
    printf("\n------ YOUR ORDER ------\n");
    for (i = 0; i < N; i++) {
        if (cart[i] > 0) {
            printf("%2d. %-13s x %-3d = Rs.%d\n",
                   i + 1, items[i], cart[i], cart[i] * prices[i]);
        }
    }
    printf("Total items: %d\n", countItems());
}

/* Adds an item to the order */
void addItem(void) {
    int code, qty;

    showMenu();
    printf("Enter item number (1-%d): ", N);
    code = readInt();
    if (code < 1 || code > N) {
        printf("Invalid item number!\n");
        return;
    }

    printf("Enter quantity: ");
    qty = readInt();
    if (qty < 1 || qty > MAX_QTY) {
        printf("Quantity must be between 1 and %d!\n", MAX_QTY);
        return;
    }

    cart[code - 1] += qty;
    printf("%d x %s added to your order.\n", qty, items[code - 1]);
}

/* Removes an item selected by mistake */
void removeItem(void) {
    int code, qty;

    if (countItems() == 0) {
        printf("\nNothing to remove. Your order is empty.\n");
        return;
    }

    showCart();
    printf("Enter item number to remove: ");
    code = readInt();
    if (code < 1 || code > N || cart[code - 1] == 0) {
        printf("This item is not in your order!\n");
        return;
    }

    printf("You have %d x %s. How many to remove? ",
           cart[code - 1], items[code - 1]);
    qty = readInt();
    if (qty <= 0) {
        printf("Invalid quantity!\n");
        return;
    }

    if (qty >= cart[code - 1]) {
        cart[code - 1] = 0;
        printf("%s removed completely.\n", items[code - 1]);
    } else {
        cart[code - 1] -= qty;
        printf("%d x %s removed. %d left.\n",
               qty, items[code - 1], cart[code - 1]);
    }
}

/* End of the day report (shown only when closing the day) */
void endOfDayReport(void) {
    int i, best = -1, max = 0;

    /* find the most selling item */
    for (i = 0; i < N; i++) {
        if (soldToday[i] > max) {
            max = soldToday[i];
            best = i;
        }
    }

    printf("\n+-------- END OF DAY SALES REPORT --------+\n");
    printf("| Bills generated   : %-19d |\n", billsToday);
    printf("| Total items sold  : %-19d |\n", itemsSoldToday);
    printf("| Total sales       : Rs.%-16.2f |\n", salesToday);
    if (best == -1) {
        printf("| Most selling item : %-19s |\n", "None");
    } else {
        printf("| Most selling item : %-19s |\n", items[best]);
        printf("| Quantity sold     : %-19d |\n", max);
    }
    printf("+-----------------------------------------+\n");

    if (itemsSoldToday > 0) {
        printf("\nItem-wise sales:\n");
        for (i = 0; i < N; i++) {
            if (soldToday[i] > 0)
                printf("  %-13s : %d\n", items[i], soldToday[i]);
        }
    }
}

/* Calculates and prints the final bill */
void generateBill(int type) {
    int i, subtotal = 0, totalQty = 0, discPercent = 0, finalAmount;
    float discount, packing = 0, taxable, cgst, sgst, total, roundOff;

    /* Step 1: subtotal and total quantity */
    for (i = 0; i < N; i++) {
        subtotal += cart[i] * prices[i];
        totalQty += cart[i];
    }

    /* Step 2: discount (slab system) */
    if (subtotal >= 1000)
        discPercent = 10;
    else if (subtotal >= 500)
        discPercent = 5;
    discount = subtotal * discPercent / 100.0;

    /* Step 3: packing charge only for takeaway */
    if (type == 2)
        packing = totalQty * PACK_CHARGE;

    /* Step 4: GST on (subtotal - discount + packing) */
    taxable = subtotal - discount + packing;
    cgst = taxable * CGST_RATE / 100;
    sgst = taxable * SGST_RATE / 100;
    total = taxable + cgst + sgst;

    /* Step 5: round off to nearest rupee */
    finalAmount = (int)(total + 0.5);
    roundOff = finalAmount - total;
    if (roundOff > -0.005 && roundOff < 0.005)
        roundOff = 0;            /* avoids printing -0.00 */

    /* ---------- PRINT BILL IN RED ---------- */
    setColor(12);                /* bill text in red */

    printf("\n");
    printLine('=', 40);
    printf("%27s\n", "CAMPUS CANTEEN");
    printf("%25s\n", "TAX INVOICE");
    printf("%31s\n", "GSTIN: XXXXXXXXXXXXXXX");
    printLine('=', 40);
    printf("Bill ID    : CNT-%d\n", nextBillNo);
    if (type == 1)
        printf("Order Type : Dine-in\n");
    else
        printf("Order Type : Takeaway\n");
    printLine('-', 40);
    printf("%-14s %4s %7s %12s\n", "Item", "Qty", "Rate", "Amount");
    printLine('-', 40);

    for (i = 0; i < N; i++) {
        if (cart[i] > 0) {
            printf("%-14s %4d %7d %12.2f\n", items[i], cart[i],
                   prices[i], (float)(cart[i] * prices[i]));
        }
    }

    printLine('-', 40);
    printf("%-28s %11.2f\n", "Subtotal", (float)subtotal);
    if (discPercent > 0)
        printf("Discount @ %2d%%%14s %11.2f\n", discPercent, "", -discount);
    if (type == 2)
        printf("%-28s %11.2f\n", "Packing Charge", packing);
    printf("%-28s %11.2f\n", "Taxable Amount", taxable);
    printf("CGST @ %.1f%%%17s %11.2f\n", CGST_RATE, "", cgst);
    printf("SGST @ %.1f%%%17s %11.2f\n", SGST_RATE, "", sgst);
    printf("%-28s %11.2f\n", "Round Off", roundOff);
    printLine('=', 40);
    printf("%-28s %11.2f\n", "GRAND TOTAL (Rs.)", (float)finalAmount);
    printLine('=', 40);
    printf("Total Items: %d\n", totalQty);
    printLine('-', 40);
    printf("%32s\n", "Thank You! Visit Again :)");
    printLine('=', 40);

    setColor(10);                /* back to green for the rest of the app */

    /* ---------- SAVE TODAY'S SALES (not printed here) ---------- */
    for (i = 0; i < N; i++) {
        soldToday[i] += cart[i];
        cart[i] = 0;             /* empty cart for next customer */
    }
    itemsSoldToday += totalQty;
    salesToday += finalAmount;
    billsToday++;
    nextBillNo++;                /* next bill gets a new unique ID */
}

/* Handles one customer's order */
void newOrder(void) {
    int choice, type, i;

    for (i = 0; i < N; i++)
        cart[i] = 0;

    printf("\nOrder Type: 1. Dine-in   2. Takeaway\nEnter choice: ");
    type = readInt();
    while (type != 1 && type != 2) {
        printf("Please enter 1 or 2: ");
        type = readInt();
    }

    do {
        printf("\n------ ORDER OPTIONS ------\n");
        printf("1. Add Item\n");
        printf("2. Remove Item\n");
        printf("3. View Order\n");
        printf("4. Generate Bill\n");
        printf("5. Cancel Order\n");
        printf("Enter choice: ");
        choice = readInt();

        switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            removeItem();
            break;
        case 3:
            showCart();
            break;
        case 4:
            if (countItems() == 0) {
                printf("Add at least one item before generating the bill!\n");
                choice = 0;      /* stay in the order menu */
            } else {
                generateBill(type);
            }
            break;
        case 5:
            for (i = 0; i < N; i++)
                cart[i] = 0;
            printf("Order cancelled.\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 4 && choice != 5);
}

int main(void) {
    int choice;

    system("title Canteen Billing System");   /* window title          */
    system("color 06");                        /* green text on black   */

    do {
        system("cls");                         /* clear screen          */
        printf("\n");
        printLine('*', 40);
        printf("%31s\n", "CANTEEN BILLING SYSTEM");
        printLine('*', 40);
        printf("1. New Order\n");
        printf("2. Close Day & Exit\n");
        printf("Enter choice: ");
        choice = readInt();

        switch (choice) {
        case 1:
            newOrder();
            printf("\nPress Enter for next customer...");
            getchar();                         /* wait before clearing  */
            break;
        case 2:
            system("cls");
            printf("\nDay closed.\n");
            endOfDayReport();
            break;
        default:
            printf("Invalid choice!");
            pressEnter();
        }
    } while (choice != 2);

    printf("\nPress Enter to close the app...");
    getchar();                                 /* keeps window open     */
    return 0;
}