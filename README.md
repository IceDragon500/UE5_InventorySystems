# UE5_Inventory
 自学路径
### 第1节 介绍 (Introduction)

✅1. Welcome to the Course! 欢迎来到本课程！

✅2. 1 - Section 1 Intro 第1节介绍

✅3. Project Creation and Assets 项目创建与资产导入

✅4. Custom Character Mesh 自定义角色网格体

✅5. Creating a Plugin 创建插件

✅6. Inventory Player Controller 库存玩家控制器

✅7. Primary Interact Action 主要交互动作

✅8. The HUD Widget HUD 控件

✅9. Item Trace Channel 物品追踪通道

✅10. Tracing for Items 追踪物品

✅11. Pickup Message 拾取信息

✅12. Highlight Material 高亮材质


### 第2节 库存HUD (Inventory HUD)

✅13. Section 2 Intro 第2节介绍

✅14. Inventory Component 库存组件

✅15. Toggle Inventory 切换库存界面

✅16. Close Button 关闭按钮

✅17. Inventory Grid 库存网格

18. Switcher Buttons 切换器按钮

19. Grid Slots 网格槽位


### 第3节 库存数据 (Inventory Data)

20. Section 3 Intro 第3节介绍

21. Fast Array Serializer 快速数组序列化器

22. Fast Array Functions 快速数组函数

23. No Room in Inventory 库存空间不足

24. Has Room for Item 是否有空间存放物品

25. Add Item RPCs 添加物品的RPC（远程过程调用）

26. Item Manifest 物品清单

27. Item Type Tags 物品类型标签

28. On Item Added 物品添加事件


### 第4节 物品片段 (Item Fragments)

29. Section 4 Intro 第4节介绍

30. Item Fragment 物品片段

31. Image Fragment 图像片段

32. Inventory Grid Check 库存网格检查

33. Add Item To Indices 添加物品到索引位置

34. Get Fragment Utilities 获取片段工具函数

35. Slotted Item 槽位物品（UI元素）

36. Add Slotted Item to Canvas 添加槽位物品到画布

37. Grid Slot Textures 网格槽位纹理

38. For Each 2D 二维循环遍历

39. Stackable Fragment 可堆叠片段

40. Stack Count 堆叠数量

41. Updating Grid Slots 更新网格槽位


### 第5节 寻找归属之地 (Searching for a Place to Belong)

42. Has Room For Item - The Plan 判断是否有空间存放物品 - 计划

43. Is the Item Stackable 物品是否可堆叠

44. How Many Stacks to Add 需要添加多少堆叠

45. Exiting the Loop Early 提前退出循环

46. Is Index Claimed 索引位置是否已被占用

47. Has Room at Index 在索引位置是否有空间

48. Check Slot Constraints 检查槽位约束

49. Is Index Claimed...Again 索引位置是否已被占用...再次检查

50. Is Upper Left Grid Slot 是否为左上角网格槽位

51. Is Stackable 是否可堆叠

52. Does Item Type Match 物品类型是否匹配

53. Is Slot at Max Capacity 槽位是否已达最大容量

54. Is In Grid Bounds 是否在网格边界内

55. Determine Fill Amount for Slot 确定槽位填充量

56. Filling in the Results 填充结果

57. Find First Item by Type 按类型查找首个物品

58. Testing our Algorithm 测试我们的算法


### 第6节 添加堆叠 (Adding Stacks)

59. Total Stack Count 总堆叠数量

60. On Picked Up 拾取事件

61. On Stack Change 堆叠数量变更事件

62. Add Stacks 添加堆叠


### 第7节 拾取 (Pick Up)

63. Section 7 Intro 第7节介绍

64. Hover Item 悬停物品（UI元素）

65. Slotted Item Clicked 槽位物品被点击

66. Pointer Events 指针事件

67. Assign Hover Item 分配悬停物品

68. Hover Item Stack Count 悬停物品堆叠数量

69. Remove Item from Grid 从网格中移除物品


### 第8节 悬停物品 (Hover Item)

70. Section 8 Intro 第8节介绍

71. Tile Parameters 网格块参数

72. Canvas and Mouse Position 画布与鼠标位置

73. Hovered Coordinates 悬停坐标

74. Calculate Tile Quadrant 计算网格块象限

75. On Tile Parameters Updated 网格块参数更新事件

76. Hover Item Dimensions 悬停物品尺寸

77. Calculate Starting Coordinate 计算起始坐标

78. Check Hover Position 检查悬停位置

79. In Grid Bounds 在网格边界内

80. Has Swappable Item 存在可交换物品

81. Cursor Exited Canvas 光标离开画布

82. Highlighting and UnHighlighting 高亮与取消高亮

83. Change Hover Type 更改悬停类型


### 第9节 放置 (Put Down)

84. Section 9 Intro 第9节介绍

85. Grid Slot Events 网格槽位事件

86. On Grid Slot Clicked 网格槽位被点击事件

87. Put Down on Index 在索引位置放置物品

88. Show and Hide Cursor 显示与隐藏光标

89. On Slotted Item Clicked 槽位物品被点击事件

90. Swap With Hover Item 与悬停物品交换

91. Should Swap Stack Counts 是否交换堆叠数量

92. Swap Stack Counts 交换堆叠数量

93. Consume Hover Item Stacks 消耗悬停物品堆叠

94. Filling in Stacks 填充堆叠

95. Slot is Full 槽位已满


### 第10节 弹出菜单 (Pop Up Menu)

96. Section 10 Intro 第10节介绍

97. Pop Up Menu Class 弹出菜单类

98. Pop Up Widget Events 弹出控件事件

99. Item Pop Up Delegates 物品弹出菜单委托

100. Finishing the Item Pop Up Class 完成物品弹出菜单类

101. Item Pop Up Blueprint 物品弹出菜单蓝图

102. Create Item Pop Up 创建物品弹出菜单

103. Weak Pointer to Item Pop Up 指向物品弹出菜单的弱指针

104. Split Drop and Consume Callbacks 拆分、丢弃和消耗的回调函数

105. On Pop Up Menu Split 弹出菜单拆分事件


### 第11节 丢弃物品 (Dropping Items)

106. Section 11 Intro 第11节介绍

107. Drop Item 丢弃物品

108. Server Drop Item 服务器端丢弃物品（RPC）

109. Spawn Dropped Item 生成被丢弃的物品

110. Spawn Pickup Actor 生成可拾取Actor

111. Test Dropping Items 测试丢弃物品

112. Dropping Items Manually 手动丢弃物品


### 第12节 消耗物品 (Consuming Items)

113. Section 12 Intro 第12节介绍

114. On Pop Up Menu Consume 弹出菜单消耗事件

115. Server Consume Item 服务器端消耗物品（RPC）

116. Consumable Fragments 可消耗物品片段


### 第13节 物品描述 (Item Description)

117. Section 13 Intro 第13节介绍

118. Item Description Widget 物品描述控件

119. Hover and Unhover for Slotted Items 槽位物品的悬停与取消悬停

120. Creating an Item Description 创建物品描述

121. Set Item Description Size and Position 设置物品描述大小和位置


### 第14节 组合模式 (The Composite Pattern)

122. The Composite Pattern 组合模式

123. Setting up the Composite Pattern 设置组合模式

124. Assimilate Inventory Fragments 整合库存片段

125. The Image Fragment 图像片段

126. The Text Fragment 文本片段

127. Labeled Number Fragment 带标签的数字片段

128. Consume Modifiers 消耗效果修饰器

129. Completing the Item Description 完成物品描述



### 第15节 装备物品 (Equipping Items)

130. Section 15 Intro 第15节介绍

131. Equipped Grid Slots 装备网格槽位

132. Hover Functionality 悬停功能

133. Equipped Grid Slot Blueprints 装备网格槽位蓝图

134. Grayed Out Icon 灰显图标

135. Adding Equipped Grid Slots 添加装备网格槽位

136. Creating an Equippable Item 创建可装备物品

137. Equipment Fragment 装备片段

138. Can Equip Hover Item 能否装备悬停物品

139. Equipped Slotted Item 已装备槽位物品（UI元素）

140. Slotted Item Draw Size 槽位物品绘制尺寸

141. Creating the Slotted Item Widget 创建槽位物品控件

142. Finishing OnItemEquipped 完成物品装备事件

143. Creating the Equipped Slotted Item 创建已装备槽位物品

144. Equipping the Cloak 装备斗篷

145. Planning EquippedSlottedItemClicked 规划已装备槽位物品点击事件

146. Items to Equip and Unequip 需要装备和卸下的物品

147. Remove Equipped Slotted Item 移除已装备槽位物品

148. Make Equipped Slotted Item 创建已装备槽位物品

149. Broadcast Slot Clicked Delegates 广播槽位点击委托

150. Edge Case - Swapping with Valid Items 边界情况 - 与有效物品交换

151. Edge Case - Switching Categories 边界情况 - 切换类别


### 第16节 装备组件 (Equipment Component)

152. Equipment Component 装备组件

153. Calling Equip Functions 调用装备函数

154. Equip Actor 装备Actor

155. Spawning the Equip Actor 生成装备Actor

156. Destroying the Equip Actor 销毁装备Actor

157. Timing Actor Lifecycles Actor生命周期时机

158. Proxy Mesh Actor 代理网格体Actor

159. Timing the Proxy Mesh 代理网格体时机

160. Proxy Mesh Blueprint 代理网格体蓝图

161. Character Display 角色展示

162. Character Display Blueprint 角色展示蓝图

163. Hiding the Dot 隐藏原点（或标记点）

164. Character Mesh Collision 角色网格体碰撞

165. Equipped Item Description 已装备物品描述

166. Creating More Items 创建更多物品

167. Bonus Lecture: Where to go from here 附加讲座：后续学习方向