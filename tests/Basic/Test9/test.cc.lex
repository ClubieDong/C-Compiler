     Token        Matched       Row  ColStart    ColEnd

       INT            int         1         1         4
   ID_TEXT        getchar         1         5        12
      CHAR              (         1        12        13
      CHAR              )         1        13        14
      CHAR              ;         1        14        15
       INT            int         2         1         4
   ID_TEXT        putchar         2         5        12
      CHAR              (         2        12        13
       INT            int         2        13        16
   ID_TEXT             ch         2        17        19
      CHAR              )         2        19        20
      CHAR              ;         2        20        21
      VOID           void         4         1         5
   ID_TEXT         output         4         6        12
      CHAR              (         4        12        13
       INT            int         4        13        16
   ID_TEXT              n         4        17        18
      CHAR              )         4        18        19
      CHAR              {         5         1         2
        IF             if         6         5         7
      CHAR              (         6         8         9
   ID_TEXT              n         6         9        10
      CHAR              <         6        11        12
  CONSTINT              0         6        13        14
      CHAR              )         6        14        15
      CHAR              {         7         5         6
   ID_TEXT        putchar         8         9        16
      CHAR              (         8        16        17
 CONSTCHAR            '-'         8        17        20
      CHAR              )         8        20        21
      CHAR              ;         8        21        22
   ID_TEXT              n         9         9        10
      CHAR              =         9        11        12
      CHAR              -         9        13        14
   ID_TEXT              n         9        14        15
      CHAR              ;         9        15        16
      CHAR              }        10         5         6
      ELSE           else        11         5         9
        IF             if        11        10        12
      CHAR              (        11        13        14
   ID_TEXT              n        11        14        15
        EQ             ==        11        16        18
  CONSTINT              0        11        19        20
      CHAR              )        11        20        21
      CHAR              {        12         5         6
   ID_TEXT        putchar        13         9        16
      CHAR              (        13        16        17
 CONSTCHAR            '0'        13        17        20
      CHAR              )        13        20        21
      CHAR              ;        13        21        22
    RETURN         return        14         9        15
      CHAR              ;        14        15        16
      CHAR              }        15         5         6
       INT            int        16         5         8
   ID_TEXT            num        16         9        12
      CHAR              [        16        12        13
  CONSTINT             20        16        13        15
      CHAR              ]        16        15        16
      CHAR              ,        16        16        17
   ID_TEXT            idx        16        18        21
      CHAR              =        16        22        23
  CONSTINT              0        16        24        25
      CHAR              ;        16        25        26
     WHILE          while        17         5        10
      CHAR              (        17        11        12
   ID_TEXT              n        17        12        13
        NE             !=        17        14        16
  CONSTINT              0        17        17        18
      CHAR              )        17        18        19
      CHAR              {        18         5         6
   ID_TEXT            num        19         9        12
      CHAR              [        19        12        13
   ID_TEXT            idx        19        13        16
      CHAR              ]        19        16        17
      CHAR              =        19        18        19
   ID_TEXT              n        19        20        21
      CHAR              %        19        22        23
  CONSTINT             10        19        24        26
      CHAR              ;        19        26        27
   ID_TEXT              n        20         9        10
DIV_ASSIGN             /=        20        11        13
  CONSTINT             10        20        14        16
      CHAR              ;        20        16        17
       INC             ++        21         9        11
   ID_TEXT            idx        21        11        14
      CHAR              ;        21        14        15
      CHAR              }        22         5         6
       FOR            for        23         5         8
      CHAR              (        23         9        10
       DEC             --        23        10        12
   ID_TEXT            idx        23        12        15
      CHAR              ;        23        15        16
   ID_TEXT            idx        23        17        20
        GE             >=        23        21        23
  CONSTINT              0        23        24        25
      CHAR              ;        23        25        26
       DEC             --        23        27        29
   ID_TEXT            idx        23        29        32
      CHAR              )        23        32        33
   ID_TEXT        putchar        24         9        16
      CHAR              (        24        16        17
   ID_TEXT            num        24        17        20
      CHAR              [        24        20        21
   ID_TEXT            idx        24        21        24
      CHAR              ]        24        24        25
      CHAR              +        24        26        27
 CONSTCHAR            '0'        24        28        31
      CHAR              )        24        31        32
      CHAR              ;        24        32        33
      CHAR              }        25         1         2
       INT            int        27         1         4
   ID_TEXT          input        27         5        10
      CHAR              (        27        10        11
      CHAR              )        27        11        12
      CHAR              {        28         1         2
       INT            int        29         5         8
   ID_TEXT              n        29         9        10
      CHAR              =        29        11        12
  CONSTINT              0        29        13        14
      CHAR              ,        29        14        15
   ID_TEXT              t        29        16        17
      CHAR              =        29        18        19
   ID_TEXT        getchar        29        20        27
      CHAR              (        29        27        28
      CHAR              )        29        28        29
      CHAR              ;        29        29        30
     WHILE          while        30         5        10
      CHAR              (        30        11        12
 CONSTCHAR            '0'        30        12        15
        LE             <=        30        16        18
   ID_TEXT              t        30        19        20
      CHAR              &        30        21        22
   ID_TEXT              t        30        23        24
        LE             <=        30        25        27
 CONSTCHAR            '9'        30        28        31
      CHAR              )        30        31        32
      CHAR              {        31         5         6
   ID_TEXT              n        32         9        10
      CHAR              =        32        11        12
   ID_TEXT              n        32        13        14
      CHAR              *        32        15        16
  CONSTINT             10        32        17        19
      CHAR              +        32        20        21
   ID_TEXT              t        32        22        23
      CHAR              -        32        24        25
 CONSTCHAR            '0'        32        26        29
      CHAR              ;        32        29        30
   ID_TEXT              t        33         9        10
      CHAR              =        33        11        12
   ID_TEXT        getchar        33        13        20
      CHAR              (        33        20        21
      CHAR              )        33        21        22
      CHAR              ;        33        22        23
      CHAR              }        34         5         6
    RETURN         return        35         5        11
   ID_TEXT              n        35        12        13
      CHAR              ;        35        13        14
      CHAR              }        36         1         2
       INT            int        38         1         4
   ID_TEXT      Fibonacci        38         5        14
      CHAR              (        38        14        15
       INT            int        38        15        18
   ID_TEXT             x1        38        19        21
      CHAR              )        38        21        22
      CHAR              {        39         1         2
       INT            int        40         7        10
   ID_TEXT            val        40        11        14
      CHAR              ;        40        14        15
        IF             if        42         2         4
      CHAR              (        42         5         6
   ID_TEXT             x1        42         6         8
        EQ             ==        42         9        11
  CONSTINT              1        42        12        13
      CHAR              )        42        13        14
   ID_TEXT            val        42        15        18
      CHAR              =        42        19        20
  CONSTINT              1        42        21        22
      CHAR              ;        42        22        23
        IF             if        43         3         5
      CHAR              (        43         6         7
   ID_TEXT             x1        43         7         9
        EQ             ==        43        10        12
  CONSTINT              2        43        13        14
      CHAR              )        43        14        15
   ID_TEXT            val        43        16        19
      CHAR              =        43        20        21
  CONSTINT              1        43        22        23
      CHAR              ;        43        23        24
        IF             if        44         2         4
      CHAR              (        44         5         6
   ID_TEXT             x1        44         6         8
      CHAR              >        44         9        10
  CONSTINT              2        44        11        12
      CHAR              )        44        12        13
   ID_TEXT            val        44        14        17
      CHAR              =        44        18        19
   ID_TEXT      Fibonacci        44        20        29
      CHAR              (        44        29        30
   ID_TEXT             x1        44        30        32
      CHAR              -        44        32        33
  CONSTINT              2        44        33        34
      CHAR              )        44        34        35
      CHAR              +        44        35        36
   ID_TEXT      Fibonacci        44        36        45
      CHAR              (        44        45        46
   ID_TEXT             x1        44        46        48
      CHAR              -        44        48        49
  CONSTINT              1        44        49        50
      CHAR              )        44        50        51
      CHAR              ;        44        51        52
    RETURN         return        45         8        14
   ID_TEXT            val        45        15        18
      CHAR              ;        45        18        19
      CHAR              }        46         1         2
      VOID           void        48         1         5
   ID_TEXT           main        48         6        10
      CHAR              (        48        10        11
      VOID           void        48        11        15
      CHAR              )        48        15        16
      CHAR              {        49         1         2
       INT            int        50         9        12
   ID_TEXT              x        50        13        14
      CHAR              ;        50        14        15
       INT            int        51         9        12
   ID_TEXT              y        51        13        14
      CHAR              ;        51        14        15
       INT            int        52         9        12
   ID_TEXT         result        52        13        19
      CHAR              ;        52        19        20
   ID_TEXT              x        54         3         4
      CHAR              =        54         5         6
  CONSTINT              5        54         7         8
      CHAR              ;        54         8         9
   ID_TEXT              y        55         3         4
      CHAR              =        55         5         6
  CONSTINT              2        55         7         8
      CHAR              ;        55         8         9
   ID_TEXT         result        56         9        15
      CHAR              =        56        16        17
   ID_TEXT      Fibonacci        56        18        27
      CHAR              (        56        27        28
   ID_TEXT              x        56        28        29
      CHAR              *        56        29        30
   ID_TEXT              y        56        30        31
      CHAR              )        56        31        32
      CHAR              ;        56        32        33
   ID_TEXT         output        57         9        15
      CHAR              (        57        15        16
   ID_TEXT         result        57        16        22
      CHAR              )        57        22        23
      CHAR              ;        57        23        24
      CHAR              }        58         1         2