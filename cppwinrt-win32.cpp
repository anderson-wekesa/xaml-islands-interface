// cppwinrt-win32.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "cppwinrt-win32.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline void InitializeWinRT();

HWND hMainWindow;
HWND hWndXamlIsland = nullptr;
RECT rMainRect;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// The main window class name.
	const wchar_t CLASS_NAME[] = L"winrt-test-class";
	WNDCLASS windowClass = {};

	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&windowClass))
	{
		MessageBoxW(NULL, L"Failed to Register Class!", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		L"C++/Win32 Test",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hMainWindow)
	{
		MessageBoxW(NULL, L"Failed to Create Window!", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//InitializeWinRT();

	winrt::init_apartment(apartment_type::single_threaded);
	WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

	DesktopWindowXamlSource xs;
	auto interopDetail = xs.as<IDesktopWindowXamlSourceNative>();
	check_hresult(interopDetail->AttachToWindow(hMainWindow));

	check_hresult(interopDetail->get_WindowHandle(&hWndXamlIsland));

	GetClientRect(hMainWindow, &rMainRect);

	SetWindowPos(hWndXamlIsland, 0, 0, 0, (rMainRect.right), (rMainRect.bottom), SWP_SHOWWINDOW);

	winrt::param::hstring str(LR"(
		<Grid Name="MainGrid" xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
			xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
			<Pivot HorizontalAlignment="Stretch" Margin="48,32,48,42" Title="Messanger" VerticalAlignment="Stretch">
				<PivotItem Header="Chats">
					<Grid>
						<SplitView x:Name="splitView" PaneBackground="{ThemeResource SystemControlBackgroundChromeMediumLowBrush}"
			   IsPaneOpen="True" OpenPaneLength="256" CompactPaneLength="48" DisplayMode="Inline">
							<SplitView.Pane>
								<Grid>
									<Grid.RowDefinitions>
										<RowDefinition Height="Auto"/>
										<RowDefinition Height="*"/>
										<RowDefinition Height="Auto"/>
									</Grid.RowDefinitions>
									<TextBlock Text="CONTACTS" x:Name="PaneHeader" Margin="10,12,0,0" Style="{StaticResource BaseTextBlockStyle}"/>
									<ListView x:Name="NavLinksList" Margin="0,12,0,0" SelectionMode="Single" Grid.Row="1" VerticalAlignment="Stretch">
										<PersonPicture DisplayName="Betsy Sherman" HorizontalAlignment="Stretch" Height="70" VerticalAlignment="Stretch" Width="49" Margin="0, 10, 0, 0"/>
										<PersonPicture DisplayName="James Doe" HorizontalAlignment="Stretch" Height="70" VerticalAlignment="Stretch" Width="49" Margin="0, 10, 0, 0"/>
										<PersonPicture DisplayName="Harry Charles" HorizontalAlignment="Stretch" Height="70" VerticalAlignment="Stretch" Width="49" Margin="0, 10, 0, 0"/>
										<PersonPicture DisplayName="Bruce Andersson" HorizontalAlignment="Stretch" Height="70" VerticalAlignment="Stretch" Width="49" Margin="0, 10, 0, 0"/>
										<PersonPicture DisplayName="John Smith" HorizontalAlignment="Stretch" Height="70" VerticalAlignment="Stretch" Width="49" Margin="0, 10, 0, 0"/>
									</ListView>

									<StackPanel Orientation="Horizontal" Grid.Row="2" Margin="14,24,0,24" >
										<SymbolIcon Symbol="Setting" />
										<TextBlock Text="Settings" Margin="24,0,0,0" VerticalAlignment="Center"/>
									</StackPanel>
								</Grid>
							</SplitView.Pane>

							<Grid>
								<Grid.RowDefinitions>
									<RowDefinition Height="Auto"/>
									<RowDefinition Height="*"/>
								</Grid.RowDefinitions>
								<TextBlock Text="CHATS" Margin="12,12,0,0" Style="{StaticResource BaseTextBlockStyle}"/>
								<TextBlock x:Name="content" Grid.Row="1" Margin="12,12,0,0" Style="{StaticResource BodyTextBlockStyle}" />
							</Grid>
						</SplitView>
					</Grid>
				</PivotItem>
				<PivotItem Header="Audio Call">
					<Grid></Grid>
				</PivotItem>
				<PivotItem Header="Video Call">
					<Grid></Grid>
				</PivotItem>
			</Pivot>
		</Grid>
	)");

	IInspectable ins = XamlReader::Load(str);
	xs.Content(ins.as<UIElement>());



	//Register Click Event Handlers
	/*
	ins.as<Grid>().FindName(L"button").as<Button>().Click([](const IInspectable& sender, const RoutedEventArgs&)
	{
		MessageBox(NULL, L"Second Button Clicked", L"Check", MB_OK | MB_ICONINFORMATION);
	});
	*/
	ShowWindow(hMainWindow, nCmdShow);

	//Message loop:
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

		case WM_PAINT:
		{
			PAINTSTRUCT ps = {};
			HDC hdc = BeginPaint(hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_SIZE:
		{
			GetClientRect(hMainWindow, &rMainRect);
			SetWindowPos(hWndXamlIsland, 0, 0, 0, (rMainRect.right), (rMainRect.bottom), SWP_SHOWWINDOW);
		}
		break;

		case WM_DESTROY:
		{
			DestroyWindow(hWnd);
		}
		break;

	}


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



inline void InitializeWinRT()
{
	winrt::init_apartment(apartment_type::single_threaded);
	WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

	DesktopWindowXamlSource xs;
	auto interopDetail = xs.as<IDesktopWindowXamlSourceNative>();
	check_hresult(interopDetail -> AttachToWindow(hMainWindow));

	check_hresult(interopDetail -> get_WindowHandle(&hWndXamlIsland));

	GetClientRect(hMainWindow, &rMainRect);

	SetWindowPos(hWndXamlIsland, 0, 0, 0, (rMainRect.right), (rMainRect.bottom), SWP_SHOWWINDOW);

	winrt::param::hstring str(LR"(
		<Grid Name="MainGrid" xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
			xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
			<Button x:Name="button" Content="First Button" HorizontalAlignment="Left" Height="40" Margin="160,134,0,0" VerticalAlignment="Top" Width="166"/>
			<Button x:Name="button1" Content="Second Button" Click="Button_Click" HorizontalAlignment="Left" Height="40" Margin="586,134,0,0" VerticalAlignment="Top" Width="140"/>
		</Grid>
	)");

	IInspectable ins = XamlReader::Load(str);
	xs.Content(ins.as<UIElement>());



	//Register Click Event Handlers

	ins.as<Grid>().FindName(L"button").as<Button>().Click([](const IInspectable& sender, const RoutedEventArgs&)
	{
		MessageBox(NULL, L"Second Button Clicked", L"Check", MB_OK | MB_ICONINFORMATION);
	});
}