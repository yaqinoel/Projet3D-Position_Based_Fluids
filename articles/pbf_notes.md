# Position Based Fluids

## 总体介绍
+ **先预测：** 把重力等外力计入速度，预测例子的新位置，并在这些位置上查找邻居
+ **再修正：** 每轮根据当前位置估计密度，计算所有粒子的位置修正，再统一更新位置。处理碰撞，多轮迭代，让密度逐渐接近目标值
+ **最后更新速度：** 用修正后的位置和旧位置之间的位移除以时间步长，再对速度做后处理

## Abstract
维持流体的不可压缩性对真实感重要，但是计算成本高，缩小时间步可以提高效率，但是不能实时应用

可以集成到基于位置的动力学PBD框架中的迭代密度求解器。
构造并求解一组用于位置恒定密度的位置约束。
该方法继承基于位置的几何方法的稳定性，可以采用适合实时应用的较大时间步长

引入人工压力项，改善粒子分布，产生表面张力效果。
在速度后处理阶段采用涡量约束，缓解能量损失

## Introduction
本文展示如何在基于PBD框架中模拟不可压缩流，它的时间积分具有无条件稳定性

### SPH 光滑粒子流体动力学

优点：质量守恒，拉格朗日离散，概念简单
缺点：不鲁棒，粒子邻居太少，无法准确估计密度，只能缩小时间步长，或者使用足够多的粒子（增加计算量）

*拉格朗日离散*
每个粒子代表一小份流体，带着自己的质量、位置和速度

## 相关工作
- SPH
- FLIP 网格上求解压力,再把速度变化传回粒子
- PBD 一种基于Verlet积分，面向游戏的动力学模拟，直接更新位置，通过迭代求解一组非线性约束

Gauss-Seidel: 先更新A，再处理B，B计算时已经能用上A的新位置
Jacobi: 让A和B都用这一轮开始时的位置来计算修正，等全部算完，再统一更新

Jacobi可以并行计算，但是一处修正需要经过多轮才能传到更远的粒子。

## Enforcing Imcompressibility 维持不可压缩性

回答一个具体问题： 如果某处的水被挤压得太密，应该移动哪些粒子，每个粒子又该移动多少？

主旨：为了维持恒定密度，为每个粒子设置一个非线性密度约束。每个约束同时取决于该粒子和它邻居的位置。求解时，先根据当前位置估计密度，再沿着约束的梯度计算位置修正。一个粒子通常会收到多条约束的影响，所以要把这些修正加起来。

### 密度估计函数（SPH核函数）
$\rho_i$ 通过SPH核函数估计出来

$$
\rho_i = \sum_j m_j W(p_i - pj, h)
$$

$m_j$是邻居粒子质量，
$W$是随距离衰减的权重，离$i$越近的粒子，通常对$\rho_i$的贡献越大。距离超过核函数的作用半径$h$,贡献就是0。论文假设粒子质量相同。

### 非线性密度约束

$$
C_i(p_i,...p_n) = \frac{\rho_i}{\rho_0} - 1
$$

$\rho_0$是目标密度（静止密度），$\rho_i$是粒子$i$所在位置的估计密度,求解器希望把$C_i$推向0。

为什么这里写了一组位置，因为虽然$C_i$衡量的是粒子$i$处的密度，但是移动$i$的任意一个邻居，痘坑呢个改变这个密度。所以约束归粒子$i$所有，修正却可以分摊给多个粒子。

### 单条约束求解

把参与一条约束的所有粒子坐标合在一起，记作$p$, 我们希望找到一组小位移$\Delta_p$, 使调整后的约束满足:
$$
C(p + \Delta p) = 0
$$

密度随位置的变化是非线性的，作者先在当前位置做一阶近似：

$$
C(p + \Delta p) \approx C(p) + \nabla C^T \Delta p
$$

右边的第二项在估计：如果这些粒子各移动一点，密度误差大概会改变多少？对于每个粒子而言，梯度告诉我们它沿哪个方向移动，最容易改变这些约束.

PBD沿梯度修正$\Delta p \approx \lambda \nabla C$

代入得到：
$$
C(p + \Delta p) \approx C(p) + \nabla C^T \lambda \nabla C
$$

代入求解目标得到
$$
0 \approx C + \lambda \nabla C^T \nabla C
$$

最终得到

$$
\begin{aligned}
\lambda_i &=
-\frac{C_i}{\displaystyle\sum_k
  \left\|\nabla_{\mathbf p_k}C_i\right\|^2}.
\end{aligned}
$$

这里的分母是梯度对自身的点积


### 单个粒子梯度求解

$$
\rho_i=\sum_j m_j W(\mathbf p_i-\mathbf p_j,h)
$$

**计算密度的核函数**

$$
W_{\mathrm{Poly6}}(\mathbf r,h)=
\begin{cases}
\dfrac{315}{64\pi h^9}(h^2-r^2)^3, & 0\le r<h,\\[4pt]
0, & r\ge h.
\end{cases}
$$

**梯度和位置修正核函数**
$$
\nabla W_{\mathrm{Spiky}}(\mathbf r,h)
=-\frac{45}{\pi h^6}(h-r)^2

$$

作者分别求约束$C_i$对中心粒子和对邻居位置的导数。
$\nabla W(p_i - p_j, h)$表示$W$对相对位置$p_i - p_j$的梯度。

#### 约束$C_i$的中心粒子$p_i$梯度

$$
\nabla_{\mathbf p_i} C_i
=\frac{1}{\rho_0}\sum_j \nabla W(\mathbf p_i-\mathbf p_j,h).
$$

#### 约束$C_i$的邻居粒子$p_j$梯度

$$
\begin{aligned}
\nabla_{\mathbf p_j}C_i
&=-\frac{1}{\rho_0}
  \nabla W(\mathbf p_i-\mathbf p_j,h),
\qquad j\ne i.
\end{aligned}
$$

### 梯度太小时,加上 $\epsilon$

当粒子快要离开彼此的作用范围时，梯度可能接近0。直觉上就是：这几个粒子稍微挪动一点，已经很难改变估计密度了，这时如果密度误差还很大，用一个很小的分母（梯度点积），就可能算出过大的修正。

#### CFM（约束力混合）软化约束

在线性化的约束力加入$\epsilon \lambda_i$

$$
C(p + \Delta p) \approx C(p) + \nabla C^T \nabla C \lambda + \epsilon \lambda
$$

解出来就得到：
$$
\lambda_i=
-\frac{C_i}{\displaystyle\sum_k
  \left\|\nabla_{\mathbf p_k}C_i\right\|^2+\varepsilon}.
$$

### 单个粒子的位置修正

把自己和邻居的修正加起来

$$
\Delta p_i = \frac{1}{\rho_0} \sum_j(\lambda_i + \lambda_j) \nabla W(p_i - pj, h)
$$

对于一对粒子i,j,括号里$\lambda_i$来自粒子$i$自己的密度约束，$\lambda_j$来自邻居$j$的密度约束，因此他们的系数可以直接相加。

实际每一轮的顺序：
估计所有粒子的密度并算出$\lambda_i$ -> 汇总每个粒子的$\Delta p_i$ -> 统一更新位置 -> 用新位置进入下一轮


## Tensible Instability 拉伸不稳定性
当某个粒子的邻居太少、无法达到静止密度时，负压力会使粒子聚成一团。

用平滑核本身定义一个人工压力项，用于把距离过近的表面粒子相互推开

$$
s_{\mathrm{corr}}
=
-k\left(
\frac{W(\mathbf p_i-\mathbf p_j,h)}
     {W(\Delta\mathbf q,h)}
\right)^n.
$$

随后把这一项加入粒子的位置更新公式：

$$
\Delta\mathbf p_i
=
\frac{1}{\rho_0}\sum_j
\left(\lambda_i+\lambda_j+s_{\mathrm{corr}}\right)
\nabla W(\mathbf p_i-\mathbf p_j,h).
$$

其中，$\Delta\mathbf q$ 表示核函数作用半径内一个固定的参考位移，$k$ 是一个较小的正常数。作者发现，取 $\Delta\mathbf q$ 为 $0.1h$ 到 $0.3h$之间的值，$k=0.1$、$n=4$，效果较好。

这个纯排斥项使估计密度低于静止密度，于是密度约束会吧邻居向内拉，形成类似表面张力的效果。





