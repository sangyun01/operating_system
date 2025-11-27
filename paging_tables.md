# Paging -- Formatted Tables Version

## Address Space

  addr(range)   page
  ------------- --------
  0 \~ 15       page 0
  16 \~ 31      page 1
  32 \~ 47      page 2
  48 \~ 63      page 3

------------------------------------------------------------------------

## Page Table Example

  VPN (index)   PFN
  ------------- -----
  0             3
  1             7
  2             5
  3             2

------------------------------------------------------------------------

## Physical Memory Example

  addr(range)   data              page frame (PFN)
  ------------- ----------------- ------------------
  0 \~ 15       reserved for OS   PFN 0
  16 \~ 31      unused            PFN 1
  32 \~ 47      page 3 of AS      PFN 2
  48 \~ 63      page 0 of AS      PFN 3
  64 \~ 79      unused            PFN 4
  80 \~ 95      page 2 of AS      PFN 5
  96 \~ 111     unused            PFN 6
  112 \~ 127    page 1 of AS      PFN 7
