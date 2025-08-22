local Players = game:GetService("Players")
local TweenService = game:GetService("TweenService")
local RunService = game:GetService("RunService")
local Player = Players.LocalPlayer
local PlayerGui = Player:WaitForChild("PlayerGui")
local Character = Player.Character or Player.CharacterAdded:Wait()
local Humanoid = Character:WaitForChild("Humanoid")

-- 状态变量
local isMinimized = false
local isRunning = false
local isJumping = false
local isNoclip = false
local noclipConnection = nil
local originalSpeed = Humanoid.WalkSpeed
local originalJumpPower = Humanoid.JumpPower
local currentSpeedMultiplier = 2
local currentJumpMultiplier = 2

-- 创建主界面容器（设置显示层级）
local mainGui = Instance.new("ScreenGui")
mainGui.Name = "ControlPanelGui"
mainGui.DisplayOrder = 100 -- 确保界面在最上层
mainGui.Parent = PlayerGui

-- 卡密验证界面
local keyFrame = Instance.new("Frame")
keyFrame.Size = UDim2.new(0, 300, 0, 200)
keyFrame.Position = UDim2.new(0.5, -150, 0.5, -100)
keyFrame.BackgroundColor3 = Color3.fromRGB(40, 40, 40)
keyFrame.BorderSizePixel = 2
keyFrame.BorderColor3 = Color3.fromRGB(80, 80, 80)
keyFrame.Parent = mainGui

local keyTitle = Instance.new("TextLabel")
keyTitle.Size = UDim2.new(1, 0, 0, 30)
keyTitle.Text = "请输入卡密"
keyTitle.TextColor3 = Color3.new(1, 1, 1)
keyTitle.BackgroundColor3 = Color3.fromRGB(60, 60, 60)
keyTitle.Parent = keyFrame

local keyInput = Instance.new("TextBox")
keyInput.Size = UDim2.new(0.8, 0, 0, 30)
keyInput.Position = UDim2.new(0.1, 0, 0.3, 0)
keyInput.PlaceholderText = "输入卡密..."
keyInput.BackgroundColor3 = Color3.fromRGB(70, 70, 70)
keyInput.TextColor3 = Color3.new(1, 1, 1)
keyInput.Parent = keyFrame

local verifyBtn = Instance.new("TextButton")
verifyBtn.Size = UDim2.new(0.4, 0, 0, 30)
verifyBtn.Position = UDim2.new(0.3, 0, 0.6, 0)
verifyBtn.Text = "验证"
verifyBtn.BackgroundColor3 = Color3.fromRGB(60, 120, 60)
verifyBtn.TextColor3 = Color3.new(1, 1, 1)
verifyBtn.Parent = keyFrame

-- 控制面板（修复透明度过渡问题）
local controlPanel = Instance.new("Frame")
controlPanel.Size = UDim2.new(0, 500, 0, 500)
controlPanel.Position = UDim2.new(0.5, -250, 0.5, -250)
controlPanel.BackgroundColor3 = Color3.fromRGB(40, 40, 40)
controlPanel.BorderSizePixel = 2
controlPanel.BorderColor3 = Color3.fromRGB(80, 80, 80)
controlPanel.Visible = false
controlPanel.BackgroundTransparency = 0 -- 初始不透明
controlPanel.Parent = mainGui

-- 面板标题栏（修复按钮位置）
local titleBar = Instance.new("Frame")
titleBar.Size = UDim2.new(1, 0, 0, 30)
titleBar.BackgroundColor3 = Color3.fromRGB(60, 60, 60)
titleBar.Parent = controlPanel

local panelTitle = Instance.new("TextLabel")
panelTitle.Size = UDim2.new(0.7, 0, 1, 0)
panelTitle.Text = "91"
panelTitle.TextColor3 = Color3.new(1, 1, 1)
panelTitle.BackgroundTransparency = 1
panelTitle.Parent = titleBar

-- 最小化按钮（修复点击区域）
local minBtn = Instance.new("TextButton")
minBtn.Size = UDim2.new(0, 30, 0, 30)
minBtn.Position = UDim2.new(1, -60, 0, 0)
minBtn.Text = "-"
minBtn.TextColor3 = Color3.new(1, 1, 1)
minBtn.BackgroundColor3 = Color3.fromRGB(80, 80, 80)
minBtn.AutoButtonColor = true -- 增加点击反馈
minBtn.Parent = titleBar

-- 关闭按钮（修复点击区域）
local closeBtn = Instance.new("TextButton")
closeBtn.Size = UDim2.new(0, 30, 0, 30)
closeBtn.Position = UDim2.new(1, -30, 0, 0)
closeBtn.Text = "X"
closeBtn.TextColor3 = Color3.new(1, 1, 1)
closeBtn.BackgroundColor3 = Color3.fromRGB(120, 40, 40)
closeBtn.AutoButtonColor = true -- 增加点击反馈
closeBtn.Parent = titleBar

-- 面板内容区域（修复显示状态）
local contentArea = Instance.new("Frame")
contentArea.Size = UDim2.new(1, 0, 1, -30)
contentArea.Position = UDim2.new(0, 0, 0, 30)
contentArea.BackgroundTransparency = 1
contentArea.Parent = controlPanel

-- 加速功能
local speedLabel = Instance.new("TextLabel")
speedLabel.Size = UDim2.new(0.3, 0, 0, 20)
speedLabel.Position = UDim2.new(0.1, 0, 0.1, 0)
speedLabel.Text = "加速倍数:"
speedLabel.TextColor3 = Color3.new(1, 1, 1)
speedLabel.BackgroundTransparency = 1
speedLabel.Parent = contentArea

local speedInput = Instance.new("TextBox")
speedInput.Size = UDim2.new(0.2, 0, 0, 30)
speedInput.Position = UDim2.new(0.4, 0, 0.09, 0)
speedInput.Text = tostring(currentSpeedMultiplier)
speedInput.TextColor3 = Color3.new(1, 1, 1)
speedInput.BackgroundColor3 = Color3.fromRGB(70, 70, 70)
speedInput.Parent = contentArea

local speedBtn = Instance.new("TextButton")
speedBtn.Size = UDim2.new(0.25, 0, 0, 30)
speedBtn.Position = UDim2.new(0.65, 0, 0.09, 0)
speedBtn.Text = "开启加速"
speedBtn.BackgroundColor3 = Color3.fromRGB(60, 60, 120)
speedBtn.TextColor3 = Color3.new(1, 1, 1)
speedBtn.Parent = contentArea

-- 跳高功能
local jumpLabel = Instance.new("TextLabel")
jumpLabel.Size = UDim2.new(0.3, 0, 0, 20)
jumpLabel.Position = UDim2.new(0.1, 0, 0.2, 0)
jumpLabel.Text = "跳高倍数:"
jumpLabel.TextColor3 = Color3.new(1, 1, 1)
jumpLabel.BackgroundTransparency = 1
jumpLabel.Parent = contentArea

local jumpInput = Instance.new("TextBox")
jumpInput.Size = UDim2.new(0.2, 0, 0, 30)
jumpInput.Position = UDim2.new(0.4, 0, 0.19, 0)
jumpInput.Text = tostring(currentJumpMultiplier)
jumpInput.TextColor3 = Color3.new(1, 1, 1)
jumpInput.BackgroundColor3 = Color3.fromRGB(70, 70, 70)
jumpInput.Parent = contentArea

local jumpBtn = Instance.new("TextButton")
jumpBtn.Size = UDim2.new(0.25, 0, 0, 30)
jumpBtn.Position = UDim2.new(0.65, 0, 0.19, 0)
jumpBtn.Text = "开启跳高"
jumpBtn.BackgroundColor3 = Color3.fromRGB(60, 120, 60)
jumpBtn.TextColor3 = Color3.new(1, 1, 1)
jumpBtn.Parent = contentArea

-- 穿墙功能
local noclipBtn = Instance.new("TextButton")
noclipBtn.Size = UDim2.new(0.5, 0, 0, 40)
noclipBtn.Position = UDim2.new(0.25, 0, 0.3, 0)
noclipBtn.Text = "开启穿墙"
noclipBtn.BackgroundColor3 = Color3.fromRGB(120, 60, 60)
noclipBtn.TextColor3 = Color3.new(1, 1, 1)
noclipBtn.Parent = contentArea

-- 卡密验证逻辑（默认卡密：test123）
verifyBtn.MouseButton1Click:Connect(function()
    if keyInput.Text == "test123" then
        local tween = TweenService:Create(
            keyFrame,
            TweenInfo.new(0.3, Enum.EasingStyle.Quad),
            {Size = UDim2.new(0, 0, 0, 0), Position = UDim2.new(0.5, 0, 0.5, 0)}
        )
        tween:Play()
        tween.Completed:Connect(function()
            keyFrame:Destroy()
            controlPanel.Visible = true
        end)
    else
        -- 错误反馈动画
        local originalColor = keyInput.BackgroundColor3
        TweenService:Create(keyInput, TweenInfo.new(0.1), {BackgroundColor3 = Color3.fromRGB(120, 40, 40)}):Play()
        task.wait(0.2)
        TweenService:Create(keyInput, TweenInfo.new(0.3), {BackgroundColor3 = originalColor}):Play()
    end
end)

-- 最小化/还原功能（核心修复）
local originalSize = controlPanel.Size
local originalPos = controlPanel.Position
minBtn.MouseButton1Click:Connect(function()
    isMinimized = not isMinimized
    local targetSize, targetPos
    
    if isMinimized then
        -- 最小化状态
        targetSize = UDim2.new(0, 200, 0, 30)
        targetPos = UDim2.new(1, -210, 0, 10)
        minBtn.Text = "+"
        contentArea.Visible = false -- 隐藏内容区
    else
        -- 还原状态
        targetSize = originalSize
        targetPos = originalPos
        minBtn.Text = "-"
        contentArea.Visible = true -- 显示内容区
    end
    
    -- 执行动画（修复动画中断问题）
    local tween = TweenService:Create(
        controlPanel,
        TweenInfo.new(0.3, Enum.EasingStyle.Quad, Enum.EasingDirection.Out),
        {Size = targetSize, Position = targetPos}
    )
    tween:Play()
end)

-- 关闭功能（核心修复）
closeBtn.MouseButton1Click:Connect(function()
    -- 还原所有角色状态
    Humanoid.WalkSpeed = originalSpeed
    Humanoid.JumpPower = originalJumpPower
    if isNoclip and noclipConnection then
        noclipConnection:Disconnect()
    end
    
    -- 关闭动画
    local tween = TweenService:Create(
        controlPanel,
        TweenInfo.new(0.3, Enum.EasingStyle.Quad),
        {Size = UDim2.new(0, 0, 0, 0), Position = UDim2.new(0.5, 0, 0.5, 0)}
    )
    tween:Play()
    tween.Completed:Connect(function()
        mainGui:Destroy() -- 彻底销毁界面
    end)
end)

-- 加速功能
speedBtn.MouseButton1Click:Connect(function()
    isRunning = not isRunning
    currentSpeedMultiplier = tonumber(speedInput.Text) or currentSpeedMultiplier
    speedInput.Text = tostring(currentSpeedMultiplier)
    
    Humanoid.WalkSpeed = isRunning and (originalSpeed * currentSpeedMultiplier) or originalSpeed
    speedBtn.Text = isRunning and "关闭加速" or "开启加速"
    speedBtn.BackgroundColor3 = isRunning and Color3.fromRGB(120, 40, 40) or Color3.fromRGB(60, 60, 120)
end)

-- 跳高功能
jumpBtn.MouseButton1Click:Connect(function()
    isJumping = not isJumping
    currentJumpMultiplier = tonumber(jumpInput.Text) or currentJumpMultiplier
    jumpInput.Text = tostring(currentJumpMultiplier)
    
    Humanoid.JumpPower = isJumping and (originalJumpPower * currentJumpMultiplier) or originalJumpPower
    jumpBtn.Text = isJumping and "关闭跳高" or "开启跳高"
    jumpBtn.BackgroundColor3 = isJumping and Color3.fromRGB(120, 40, 40) or Color3.fromRGB(60, 120, 60)
end)

-- 穿墙功能
local function enableNoclip()
    return RunService.Stepped:Connect(function()
        if Character and Character:FindFirstChild("HumanoidRootPart") then
            for _, part in ipairs(Character:GetDescendants()) do
                if part:IsA("BasePart") then
                    part.CanCollide = false
                end
            end
        end
    end)
end

noclipBtn.MouseButton1Click:Connect(function()
    isNoclip = not isNoclip
    
    if isNoclip then
        noclipConnection = enableNoclip()
        noclipBtn.Text = "关闭穿墙"
        noclipBtn.BackgroundColor3 = Color3.fromRGB(40, 120, 40)
    else
        if noclipConnection then
            noclipConnection:Disconnect()
        end
        if Character then
            for _, part in ipairs(Character:GetDescendants()) do
                if part:IsA("BasePart") then
                    part.CanCollide = true
                end
            end
        end
        noclipBtn.Text = "开启穿墙"
        noclipBtn.BackgroundColor3 = Color3.fromRGB(120, 60, 60)
    end
end)

-- 角色重生监听
Player.CharacterAdded:Connect(function(newChar)
    Character = newChar
    Humanoid = newChar:WaitForChild("Humanoid")
    originalSpeed = Humanoid.WalkSpeed
    originalJumpPower = Humanoid.JumpPower
    
    -- 恢复状态
    if isRunning then
        Humanoid.WalkSpeed = originalSpeed * currentSpeedMultiplier
    end
    if isJumping then
        Humanoid.JumpPower = originalJumpPower * currentJumpMultiplier
    end
    if isNoclip then
        noclipConnection = enableNoclip()
    end
end)
