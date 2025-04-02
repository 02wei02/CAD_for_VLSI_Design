# Floorplanning

chip planning 介紹

### partitioning

- areas and shapes can't be modified (rigid/hard block) = size
- approximate areas and no particular shapes (flexible/soft block) = $w \times h$
- now: partitioning according to functionality

<img src="./img/2024-05-11-23-43-04.png" width="400">

### floorplanning

- location
- shape, pin location
- based on functionality
- 2-D
- resources have been determined and estimating delay
- top down design strategy
- step-wise refinement strategy = initial solution + improve

### Objectives

- minimize total chip area
- reduce wirelength
- maximize routability
- minimize congestion (track)

$$
\rm{aspect\_ratio} = \frac{h_i}{w_i}
$$

### soft block

![](./img/2024-05-12-09-45-11.png)

### hard block

<img src="./img/2024-05-12-09-45-41.png" width="300">

### Objective function

$$
Cost = \alpha A + \beta L
$$

decide by designer $\alpha , \beta$

A 是 area, B是 wirelength

A 影響比較大 $\mu m^2$ ,B 才 $nm$

floorplanning 還沒決定好 gate 要擺放的位置

希望估計 wirelength

- center-to-center estimation

- half-perimeter estimation
  
  2024Spring_EE6094_CAD_For_VLSIDesign_Chap7_Floorplanning (1) 32:09

Dead spcace

![](img/2024-05-12-12-08-49-image.png)

Dead percentage

$$
(A-\sum_i{A_i})/A\times 100
$$

### Slicing function

repetitively subdividing

vertical -> horizontal -> vertical

<img title="" src="img/2024-05-12-12-15-47-image.png" alt="" width="254"> 

### Non-slicing function

not the completed cut

<img title="" src="img/2024-05-12-12-16-35-image.png" alt="" width="259">

## Polar Graph Representation

- a pair of acyclic graphs : horizontal, vertical
  
  - Vertex: channel
  
  - Edge: 2 sides of block 

<img src="img/2024-05-12-12-24-36-image.png" title="" alt="" width="92">

- vertex -> cut

- edge -> one block (上面有標示weight)

<img title="" src="img/2024-05-12-12-26-05-image.png" alt="" width="341">

### Leaf cell

at the lowest level

it does not contain any other cell

### Composite cell

composed of leaf cells or composite cells

H is horizontal cut

V is vertical cut

<img title="" src="img/2024-05-12-15-29-14-image.png" alt="" width="507">

🟥 **Post order** = polish expression

<img src="img/2024-05-12-15-59-59-image.png" title="" alt="" width="273">

For $n$ blocks, a Polish Expression contains $n$ operands (blocks) and $n-1$ operators $(H,V)$

🟧 **Skewed Slicing Tree**

- $\rm{node}\neq \rm{right\ son}$

🟧 **Normalized Polish Expression**

- no consecutive H's or V's

<img title="" src="img/2024-05-12-16-23-17-image.png" alt="" width="295">

(the balloting property)for every subexpression $E_i = e1, e_i$,

$1≤i≤2n-1$, # operands> # operators.

Chain: HVHVH ... or VHVHV ...

adjacent : 相臨 operands(num)或 operators(V,H)

⬛️ 

- M1 (Operand Swap): Swap two adjacent operands.

- M2 (Chain Invert): Complement some chain ($\overline{V}=H$, $\overline{H}=V$). 

- M3 (Operator/Operand Swap): Swap two adjacent operand and operator.

<img title="" src="img/2024-05-12-18-31-42-image.png" alt="" width="362" data-align="center">

<img title="" src="img/2024-05-12-18-45-24-image.png" alt="" width="385" data-align="center">

### Cost Function

$$
\phi =A+\lambda W
$$

A is bounding retangle

W is overall wiring length

$\lambda$ user-specified parameter

$$
W=\Sigma_{ij}c_{ij}d_{ij}
$$

$c_{ij}$ : # of connections between i and j blocks

$d_{ij}$ : center to center distance

- Soft block : continuous regions

- Block with several existing design

<img title="" src="img/2024-05-12-18-50-38-image.png" alt="" width="436" data-align="center">

### Curve Combining

continuous 很難做

抽樣幾個可能性

同長 或 同寬 可以比較一下，刪掉大的

<img title="" src="img/2024-05-12-18-54-42-image.png" alt="" width="463" data-align="center">

每個 node 留下相對好的解答 Dynamic programming!

<img title="" src="img/2024-05-12-19-14-12-image.png" alt="" width="430" data-align="center">

k points for each shape curve, for each NPE $O(kn)$

<img title="" src="img/2024-05-12-19-18-13-image.png" alt="" width="439" data-align="center">

updating move: 只會動有變化的部分 (at most two path) $O(k \log n)$

<img title="" src="img/2024-05-12-19-22-24-image.png" alt="" width="429" data-align="center">

<img title="" src="img/2024-05-12-19-22-52-image.png" alt="" width="430" data-align="center">

<img title="" src="img/2024-05-12-19-23-03-image.png" alt="" width="432" data-align="center">

## Sequence Pair

不是  slicing floor plan 怎麼設計

Simulated Annealing

> ( 2024Spring_EE6094_CAD_For_VLSI_Design_Chap7_Floorplanning (2)53:05)

邊邊延伸

<img title="" src="img/2024-05-12-19-27-18-image.png" alt="" width="439" data-align="center">

### Locus

從中心點開始，碰到邊線，觸發下面兩種，並畫圖

Postive loci: right-up, left-down 左上到右下切 $\Gamma_+$

Negative loci: up-left, down-right 左下到右上切  $\Gamma_-$

$x'$ is after (before) $x$ in both $\Gamma_+$ , and  $\Gamma_-\Rightarrow x'$  is right (left) to $x$.

$x'$ is after (before) $x$ in $\Gamma_+$ , and $\Gamma_-\Rightarrow x'$ is before (after) to $x$. $\Rightarrow x'$ is below(above) x

cf ( f 都在 c 的右邊)

ab(b在a後面，b在a前面。所以b在a下面)

<img title="" src="img/2024-05-12-19-49-55-image.png" alt="" width="488" data-align="center">

標好 $\Gamma_+$ 和 $\Gamma_-$ 畫格子，然後轉 45 度角

source node, terminal node

longest path algorithm 找到對應的 location

<img title="" src="img/2024-05-12-20-36-35-image.png" alt="" width="446" data-align="center">

## B*-Tree

left child $\Rightarrow$ 右邊

right child $\Rightarrow$ 上面

<img title="" src="img/2024-05-12-20-42-13-image.png" alt="" width="364" data-align="center">

x coordinate: 

- Left child: the lowest, adjacent block on the right ($x_j=x_i+w_i$)

- Right child: the first block above, with the same x-coordinate ($x_j=x_i$).

y coordinate?

用 contour : 紀錄每一段 x 座標最高 y

可用 linked list

- x 左邊座標

- x 右邊座標

- y 高度

<img title="" src="img/2024-05-12-20-45-06-image.png" alt="" width="422" data-align="center">

### Preplaced module

交換

<img title="" src="img/2024-05-12-20-49-55-image.png" alt="" width="426" data-align="center">

### 引用

> 🟧🟨🟩🟦🟪⬛️⬜️🟫🟥