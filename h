local Players = game:GetService("Players")
local TweenService = game:GetService("TweenService")
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")

local player = Players.LocalPlayer
local playerGui = player:WaitForChild("PlayerGui")
local character = player.Character or player.CharacterAdded:Wait()
local humanoid = character:WaitForChild("Humanoid")

-- 功能状态变量
local speedBoost = false
local highJump = false
local noclip = false
local speedMultiplier = 2 -- 默认加速倍数
local jumpMultiplier = 2 -- 默认跳高倍数
local originalWalkSpeed = humanoid.WalkSpeed
local originalJumpPower = humanoid.JumpPower

-- 创建UI
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "PlayerCheatPanel"
screenGui.Parent = playerGui

-- 主面板
local mainPanel = Instance.new("Frame")
mainPanel.Name = "MainPanel"
mainPanel.Size = UDim2.new(0, 320, 0, 450)
mainPanel.Position = UDim2.new(0.5, -160, 0.5, -225)
mainPanel.BackgroundColor3 = Color3.new(0.2, 0.2, 0.2)
mainPanel.BorderSizePixel = 0
mainPanel.Active = true
mainPanel.Parent = screenGui

-- 标题栏
local titleBar = Instance.new("Frame")
titleBar.Name = "TitleBar"
titleBar.Size = UDim2.new(1, 0, 0, 40)
titleBar.BackgroundColor3 = Color3.new(0.3, 0.3, 0.3)
titleBar.Parent = mainPanel

local titleText = Instance.new("TextLabel")
titleText.Text = "人物功能面板"
titleText.Size = UDim2.new(0.7, 0, 1, 0)
titleText.Position = UDim2.new(0.05, 0, 0, 0)
titleText.BackgroundTransparency = 1
titleText.TextColor3 = Color3.new(1, 1, 1)
titleText.Parent = titleBar

-- 最小化按钮
local toggleButton = Instance.new("TextButton")
toggleButton.Text = "−"
toggleButton.Size = UDim2.new(0.2, 0, 1, 0)
toggleButton.Position = UDim2.new(0.8, 0, 0, 0)
toggleButton.BackgroundColor3 = Color3.new(0.4, 0.4, 0.4)
toggleButton.TextColor3 = Color3.new(1, 1, 1)
toggleButton.Parent = titleBar

-- 功能内容区域
local contentFrame = Instance.new("Frame")
contentFrame.Size = UDim2.new(1, 0, 1, -40)
contentFrame.Position = UDim2.new(0, 0, 0, 40)
contentFrame.BackgroundTransparency = 1
contentFrame.Parent = mainPanel

-- 1. 加速功能区
local speedLabel = Instance.new("TextLabel")
speedLabel.Text = "加速倍数:"
speedLabel.Size = UDim2.new(0.3, 0, 0, 30)
speedLabel.Position = UDim2.new(0.1, 0, 0.05, 0)
speedLabel.BackgroundTransparency = 1
speedLabel.TextColor3 = Color3.new(1, 1, 1)
speedLabel.Parent = contentFrame

local speedInput = Instance.new("TextBox")
speedInput.Text = tostring(speedMultiplier)
speedInput.Size = UDim2.new(0.3, 0, 0, 30)
speedInput.Position = UDim2.new(0.4, 0, 0.05, 0)
speedInput.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
speedInput.TextColor3 = Color3.new(1, 1, 1)
speedInput.Parent = contentFrame
speedInput.FocusLost:Connect(function()
    local num = tonumber(speedInput.Text)
    if num and num > 0 then
        speedMultiplier = num
        if speedBoost then
            humanoid.WalkSpeed = originalWalkSpeed * speedMultiplier
        end
    else
        speedInput.Text = tostring(speedMultiplier)
    end
end)

local speedButton = Instance.new("TextButton")
speedButton.Text = "加速: 关闭"
speedButton.Size = UDim2.new(0.8, 0, 0, 40)
speedButton.Position = UDim2.new(0.1, 0, 0.12, 0)
speedButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
speedButton.TextColor3 = Color3.new(1, 1, 1)
speedButton.Parent = contentFrame

-- 2. 跳高功能区
local jumpLabel = Instance.new("TextLabel")
jumpLabel.Text = "跳高倍数:"
jumpLabel.Size = UDim2.new(0.3, 0, 0, 30)
jumpLabel.Position = UDim2.new(0.1, 0, 0.22, 0)
jumpLabel.BackgroundTransparency = 1
jumpLabel.TextColor3 = Color3.new(1, 1, 1)
jumpLabel.Parent = contentFrame

local jumpInput = Instance.new("TextBox")
jumpInput.Text = tostring(jumpMultiplier)
jumpInput.Size = UDim2.new(0.3, 0, 0, 30)
jumpInput.Position = UDim2.new(0.4, 0, 0.22, 0)
jumpInput.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
jumpInput.TextColor3 = Color3.new(1, 1, 1)
jumpInput.Parent = contentFrame
jumpInput.FocusLost:Connect(function()
    local num = tonumber(jumpInput.Text)
    if num and num > 0 then
        jumpMultiplier = num
        if highJump then
            humanoid.JumpPower = originalJumpPower * jumpMultiplier
        end
    else
        jumpInput.Text = tostring(jumpMultiplier)
    end
end)

local jumpButton = Instance.new("TextButton")
jumpButton.Text = "跳高: 关闭"
jumpButton.Size = UDim2.new(0.8, 0, 0, 40)
jumpButton.Position = UDim2.new(0.1, 0, 0.29, 0)
jumpButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
jumpButton.TextColor3 = Color3.new(1, 1, 1)
jumpButton.Parent = contentFrame

-- 3. 穿墙按钮
local noclipButton = Instance.new("TextButton")
noclipButton.Text = "穿墙: 关闭"
noclipButton.Size = UDim2.new(0.8, 0, 0, 40)
noclipButton.Position = UDim2.new(0.1, 0, 0.39, 0)
noclipButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
noclipButton.TextColor3 = Color3.new(1, 1, 1)
noclipButton.Parent = contentFrame

-- 4. 传送功能区
local playerListLabel = Instance.new("TextLabel")
playerListLabel.Text = "选择玩家:"
playerListLabel.Size = UDim2.new(0.3, 0, 0, 30)
playerListLabel.Position = UDim2.new(0.1, 0, 0.49, 0)
playerListLabel.BackgroundTransparency = 1
playerListLabel.TextColor3 = Color3.new(1, 1, 1)
playerListLabel.Parent = contentFrame

local playerDropdown = Instance.new("TextButton")
playerDropdown.Text = "点击选择"
playerDropdown.Size = UDim2.new(0.5, 0, 0, 30)
playerDropdown.Position = UDim2.new(0.4, 0, 0.49, 0)
playerDropdown.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
playerDropdown.TextColor3 = Color3.new(1, 1, 1)
playerDropdown.Parent = contentFrame

local teleportButton = Instance.new("TextButton")
teleportButton.Text = "传送到所选玩家"
teleportButton.Size = UDim2.new(0.8, 0, 0, 40)
teleportButton.Position = UDim2.new(0.1, 0, 0.56, 0)
teleportButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
teleportButton.TextColor3 = Color3.new(1, 1, 1)
teleportButton.Parent = contentFrame

-- 变量：当前选中的玩家
local selectedPlayer = nil

-- 更新玩家列表
local function updatePlayerList()
    local players = Players:GetPlayers()
    local playerNames = {}
    for _, p in ipairs(players) do
        if p ~= player then
            table.insert(playerNames, p.Name)
        end
    end
    return playerNames
end

-- 显示玩家选择菜单
local function showPlayerMenu()
    local playerNames = updatePlayerList()
    if #playerNames == 0 then
        playerDropdown.Text = "无其他玩家"
        return
    end
    
    -- 创建临时选择菜单
    local menu = Instance.new("Frame")
    menu.Size = UDim2.new(0, 200, 0, #playerNames * 30)
    menu.Position = UDim2.new(0, playerDropdown.AbsolutePosition.X, 0, playerDropdown.AbsolutePosition.Y + 30)
    menu.BackgroundColor3 = Color3.new(0.3, 0.3, 0.3)
    menu.Parent = contentFrame
    
    for i, name in ipairs(playerNames) do
        local btn = Instance.new("TextButton")
        btn.Text = name
        btn.Size = UDim2.new(1, 0, 0, 30)
        btn.Position = UDim2.new(0, 0, 0, (i-1)*30)
        btn.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
        btn.TextColor3 = Color3.new(1, 1, 1)
        btn.Parent = menu
        btn.MouseButton1Click:Connect(function()
            selectedPlayer = Players:FindFirstChild(name)
            playerDropdown.Text = "已选: " .. name
            menu:Destroy()
        end)
        btn.TouchTap:Connect(function()
            selectedPlayer = Players:FindFirstChild(name)
            playerDropdown.Text = "已选: " .. name
            menu:Destroy()
        end)
    end
end

-- 绑定玩家选择按钮
playerDropdown.MouseButton1Click:Connect(showPlayerMenu)
playerDropdown.TouchTap:Connect(showPlayerMenu)

-- 传送功能
teleportButton.MouseButton1Click:Connect(function()
    if selectedPlayer and selectedPlayer.Character then
        local targetRoot = selectedPlayer.Character:FindFirstChild("HumanoidRootPart")
        local myRoot = character:FindFirstChild("HumanoidRootPart")
        if targetRoot and myRoot then
            myRoot.CFrame = targetRoot.CFrame * CFrame.new(0, 0, -3) -- 传送至目标前方3格
        end
    end
end)
teleportButton.TouchTap:Connect(teleportButton.MouseButton1Click)

-- 动画参数
local isMinimized = false
local originalSize = mainPanel.Size
local minimizedSize = UDim2.new(0, 320, 0, 40)
local tweenInfo = TweenInfo.new(0.3, Enum.EasingStyle.Quad, Enum.EasingDirection.InOut)

-- 最小化/还原动画
local function togglePanel()
    isMinimized = not isMinimized
    toggleButton.Text = isMinimized and "+" or "−"
    
    local sizeTween = TweenService:Create(mainPanel, tweenInfo, {
        Size = isMinimized and minimizedSize or originalSize
    })
    sizeTween:Play()
    
    local contentTween = TweenService:Create(contentFrame, tweenInfo, {
        Transparency = isMinimized and 1 or 0
    })
    contentTween:Play()
end

-- 加速功能
local function toggleSpeed()
    speedBoost = not speedBoost
    speedButton.Text = "加速: " .. (speedBoost and "开启" or "关闭")
    speedButton.BackgroundColor3 = speedBoost and Color3.new(0, 0.8, 0) or Color3.new(0.5, 0.5, 0.5)
    humanoid.WalkSpeed = speedBoost and originalWalkSpeed * speedMultiplier or originalWalkSpeed
end

-- 跳高功能
local function toggleJump()
    highJump = not highJump
    jumpButton.Text = "跳高: " .. (highJump and "开启" or "关闭")
    jumpButton.BackgroundColor3 = highJump and Color3.new(0, 0.8, 0) or Color3.new(0.5, 0.5, 0.5)
    humanoid.JumpPower = highJump and originalJumpPower * jumpMultiplier or originalJumpPower
end

-- 穿墙功能
local function toggleNoclip()
    noclip = not noclip
    noclipButton.Text = "穿墙: " .. (noclip and "开启" or "关闭")
    noclipButton.BackgroundColor3 = noclip and Color3.new(0, 0.8, 0) or Color3.new(0.5, 0.5, 0.5)
end

-- 循环检测穿墙状态
RunService.RenderStepped:Connect(function()
    if noclip and character then
        for _, part in ipairs(character:GetDescendants()) do
            if part:IsA("BasePart") then
                part.CanCollide = false
            end
        end
    elseif not noclip and character then
        for _, part in ipairs(character:GetDescendants()) do
            if part:IsA("BasePart") then
                part.CanCollide = true
            end
        end
    end
end)

-- 角色重生时重置状态
player.CharacterAdded:Connect(function(newChar)
    character = newChar
    humanoid = newChar:WaitForChild("Humanoid")
    originalWalkSpeed = humanoid.WalkSpeed
    originalJumpPower = humanoid.JumpPower
    
    -- 重置功能状态
    speedBoost = false
    highJump = false
    noclip = false
    speedButton.Text = "加速: 关闭"
    jumpButton.Text = "跳高: 关闭"
    noclipButton.Text = "穿墙: 关闭"
    speedButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
    jumpButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
    noclipButton.BackgroundColor3 = Color3.new(0.5, 0.5, 0.5)
end)

-- 绑定按钮事件
toggleButton.MouseButton1Click:Connect(togglePanel)
toggleButton.TouchTap:Connect(togglePanel)
speedButton.MouseButton1Click:Connect(toggleSpeed)
speedButton.TouchTap:Connect(toggleSpeed)
jumpButton.MouseButton1Click:Connect(toggleJump)
jumpButton.TouchTap:Connect(toggleJump)
noclipButton.MouseButton1Click:Connect(toggleNoclip)
noclipButton.TouchTap:Connect(toggleNoclip)

-- 面板拖动功能（支持触摸）
local isDragging = false
local dragStartPos = Vector2.new()
local panelStartPos = UDim2.new()

titleBar.InputBegan:Connect(function(input)
    if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
        isDragging = true
        dragStartPos = input.Position
        panelStartPos = mainPanel.Position
    end
end)

UserInputService.InputChanged:Connect(function(input)
    if isDragging and (input.UserInputType == Enum.UserInputType.MouseMovement or input.UserInputType == Enum.UserInputType.Touch) then
        local delta = input.Position - dragStartPos
        mainPanel.Position = UDim2.new(
            panelStartPos.X.Scale,
            panelStartPos.X.Offset + delta.X,
            panelStartPos.Y.Scale,
            panelStartPos.Y.Offset + delta.Y
        )
    end
end)

UserInputService.InputEnded:Connect(function(input)
    if (input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch) and isDragging then
        isDragging = false
    end
end)
