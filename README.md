[README.md](https://github.com/user-attachments/files/31220630/README.md)
# Campus Food Court Self-Order Kiosk System — Team Scaffold

CT077-3-2-DSTR | Lab Evaluation Work #2

## What this is

Shared project skeleton so all 5 members can build their module in
isolation and still integrate cleanly at the end. It contains:

- Common data types every module speaks (`include/Common.hpp`)
- One class per module with the method signatures already agreed on
(the "integration contract")
- An interactive, **menu-driven** `main.cpp` modelled as a real Campus
Food Court kiosk (student self-order flow + staff/admin panel). This
IS the integration point / demo — every module's operations are
already wired to their kiosk actions.
- A `Makefile` to build everything together

## My Contribution

I developed the Food Court Layout module using a general tree implemented with first-child and next-sibling pointers. The module supports location insertion, traversal, searching, route finding, edge-case handling and benchmark testing.


`main.cpp` already calls every module's public methods through the
kiosk menus, so as each owner fills in their `.cpp`, their part of the
kiosk comes alive with **no change to** `main.cpp`. Until then, unfinished
modules simply print their placeholder (`TODO`) output.

## Structure

```
CampusKioskSystem/
├── include/
│   └── Common.hpp              # shared structs: Order, MenuItem, Stall, SessionStep
├── src/
│   ├── OrderQueue.hpp/.cpp          # Member 1 — Queue
│   ├── StallAssignment.hpp/.cpp     # Member 2 — Circular Queue
│   ├── SessionHistory.hpp/.cpp      # Member 3 — Stack
│   ├── MenuSearch.hpp/.cpp          # Member 4 — Binary Search Tree
│   ├── FoodCourtLayout.hpp/.cpp     # Member 5 — Tree (optional module)
│   └── main.cpp                     # integration driver / demo
├── Makefile
└── README.md
```



## Work division


| Member | File(s) to own             | Module                        | Data Structure |
| ------ | -------------------------- | ----------------------------- | -------------- |
| 1      | `OrderQueue.hpp/.cpp`      | Order Queue Management        | Queue          |
| 2      | `StallAssignment.hpp/.cpp` | Stall Assignment              | Circular Queue |
| 3      | `SessionHistory.hpp/.cpp`  | Session History & Navigation  | Stack          |
| 4      | `MenuSearch.hpp/.cpp`      | Menu Item Search & Management | BST            |
| 5      | `FoodCourtLayout.hpp/.cpp` | Food Court Layout (optional)  | Tree           |




## Ground rules for integration to actually work

1. **Only edit your own** `.hpp`**/**`.cpp` **pair.** If you need a new public
  method, propose it to the team first — `main.cpp` and possibly
   another member's code may depend on the current signatures.
   (`main.cpp` is shared: its kiosk menus already call your existing
   signatures, so keep them stable. If you add a *new* public method,
   tell the team so it can be surfaced in the menu.)
2. **Don't touch** `Common.hpp` **fields without agreement** — everyone's
  code reads/writes the same `Order`, `MenuItem`, `Stall`,
   `SessionStep` structs.
3. **No STL containers** (`<vector>`, `<list>`, `<queue>`, `<stack>`,
  `<map>`, etc.) — build your own nodes/arrays. `<string>`,
   `<iostream>` etc. are fine.
4. Build and test your module on its own before plugging into
  `main.cpp` — e.g. write a tiny scratch `main()` in your own scratch
   file that only exercises your class.
5. Once your methods are implemented, `main.cpp` should compile and
  run against everyone's code with **zero changes** to `main.cpp`
   itself. If it needs a change, that's a sign the interface drifted —
   flag it to the team.



## Build

```
make        # builds ./kiosk_system
./kiosk_system
make clean  # removes the binary
```

Running the program drops you at the kiosk **WELCOME** screen:

```
1. Start ordering (I'm a student)   -> browse/search menu, place order,
                                        view map, undo steps
2. Staff / Admin panel              -> manage menu items, stalls,
                                        order queue, layout
0. Shut down kiosk
```

Menu input is validated (bad entries re-prompt) and the program exits
cleanly on end-of-input.



