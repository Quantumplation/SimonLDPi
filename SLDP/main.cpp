#include <vector>
#include "Win32++\wincore.h"
#include "GUIBase.h"
#include "GUITrack.h"
using namespace std;
using namespace SLDP;

class CView : public CWnd
{
public:
	CView() : nodeToLabel(NULL) {}
	virtual ~CView() {}
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* dc)
	{
		track.draw(*dc);
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			dc->MoveTo(lastMouseClick);
			dc->LineTo(lastMousePos);
		}
	}


private:
	GUITrack track;
	POINT lastMouseClick;
	POINT lastMousePos;
	GUINode* nodeToLabel;
};

class MyApp : public CWinApp
{
public:
	MyApp() {}
	virtual ~MyApp() {}
	virtual BOOL InitInstance()
	{
		myView.Create();
		myView.SetFont(new CFont(CreateFont(16,
			8,
			0,
			0,
			FW_DONTCARE,
			FALSE,
			FALSE,
			0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH | FF_MODERN,
			L"Courier New")));
		return TRUE;
	}
private:
	CView myView;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	MyApp myApp;
	return myApp.Run();
}

LRESULT CView::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			if (nodeToLabel)
			{
				if (wParam >= '0' && wParam <= 'Z' && wParam != 0x40 || wParam == VK_SPACE)
					nodeToLabel->addCharToLabel((wchar_t)wParam);
				else if (wParam == VK_BACK)
					nodeToLabel->removeCharFromLabel();
				else if (wParam != VK_SHIFT)
				{
					nodeToLabel->setBeingRelabeled(false);
					nodeToLabel = NULL;
				}
				InvalidateRect(NULL, true);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetCapture();
			lastMousePos.x = lastMouseClick.x = LOWORD(lParam);
			lastMousePos.y = lastMouseClick.y = HIWORD(lParam);
			track.leftPressed(lastMouseClick);
			if(nodeToLabel)
			{
				nodeToLabel->setBeingRelabeled(false);
				nodeToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (wParam == MK_LBUTTON)
			{
				lastMousePos.x = LOWORD(lParam);
				lastMousePos.y = HIWORD(lParam);
				InvalidateRect(NULL, true);
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			lastMousePos.x = LOWORD(lParam);
			lastMousePos.y = HIWORD(lParam);
			nodeToLabel = track.leftReleased(lastMousePos);
			InvalidateRect(NULL, true);
		}
		break;
	case WM_RBUTTONUP:
		{
			POINT mouseClick;
			ReleaseCapture();
			mouseClick.x = LOWORD(lParam);
			mouseClick.y = HIWORD(lParam);
			track.rightReleased(mouseClick);
			track.removeDeletedElements();
			if(nodeToLabel)
			{
				nodeToLabel->setBeingRelabeled(false);
				nodeToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_DESTROY: //When the window is destroyed
		PostQuitMessage(0);
		break;
	}
	return WndProcDefault(message, wParam, lParam);
}
