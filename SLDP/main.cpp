#include <vector>
//#include "Redirect.h"
//#include "NIDAQWrapper.h"
#include <cstdio>
#include "Win32++\wincore.h"
#include "GUIBase.h"
#include "GUITrack.h"
#include "GUINode.h"
#include "GUIButton.h"
using namespace std;
using namespace SLDP;

#define TRANS(x, y) (x*xScale)+xShift, (y*yScale)+yShift
#define EDGE(x, y) track.addEdge(new GUIEdge(x, y, (x->getLabel() + y->getLabel())))

class CView : public CWnd
{
public:
	CView() :
	  objectToLabel(NULL)
	{
		track.loadFromFile("input.xml");
		track.saveToFile("junk.xml");
		mode = NORMAL;
		buttons.push_back(new ModeButton(340, 455, 125, 50, &track));
		buttons.push_back(new TrainModeButton(340, 504, 125, 50, &mode));
		buttons.push_back(new ReadButton(490, 455, 125, 50, &track));
		buttons.push_back(new WriteButton(490, 504, 125, 50, &track));
		buttons.push_back(new SaveButton(640, 455, 125, 50, &track));
		buttons.push_back(new LoadButton(640, 504, 125, 50, &track));
		buttons.push_back(new TrainStartButton(25, 175, 125, 50, &track, &mode, track.getFirstNode("L1")));
		buttons.push_back(new TrainStartButton(25, 300, 125, 50, &track, &mode, track.getFirstNode("L2")));
		buttons.push_back(new TrainStartButton(25, 450, 125, 50, &track, &mode, track.getFirstNode("L3")));
		buttons.push_back(new TrainStartButton(975, 100, 125, 50, &track, &mode, track.getFirstNode("R1")));
		buttons.push_back(new TrainStartButton(975, 300, 125, 50, &track, &mode, track.getFirstNode("R2")));
		buttons.push_back(new TrainStartButton(975, 450, 125, 50, &track, &mode, track.getFirstNode("R3")));
	}
	virtual ~CView() {}
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* dc)
	{
		for (size_t i = 0; i < buttons.size(); ++i)
			buttons[i]->draw(*dc);
		track.draw(*dc);
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && track.getMode())
		{
			dc->MoveTo(lastMouseClick);
			dc->LineTo(lastMousePos);
		}
	}


private:
	GUITrack track;
	POINT lastMouseClick;
	POINT lastMousePos;
	GUIBase* objectToLabel;
	TrainMode mode;
	vector<GUIButton*> buttons;
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
//	NIDAQWrapper wrapper;
//	wrapper.Initialize();
	MyApp myApp;
	return myApp.Run();
}

LRESULT CView::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			if (objectToLabel)
			{
				if (wParam >= '0' && wParam <= 'Z' && wParam != 0x40 || wParam == VK_SPACE)
					objectToLabel->addCharToLabel((wchar_t)wParam);
				else if (wParam == VK_BACK)
					objectToLabel->removeCharFromLabel();
				else if (wParam != VK_SHIFT)
				{
					objectToLabel->setBeingRelabeled(false);
					objectToLabel = NULL;
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
			bool clickedOnButton = false;
			for (size_t i = 0; i < buttons.size(); ++i)
				if (buttons[i]->contains(lastMouseClick))
				{
					buttons[i]->onClick();
					clickedOnButton = true;
				}
			if (!clickedOnButton)
				track.leftPressed(lastMouseClick);
			if(objectToLabel)
			{
				objectToLabel->setBeingRelabeled(false);
				objectToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (wParam == MK_LBUTTON && track.getMode())
			{
				long oldMouseX = lastMousePos.x;
				long oldMouseY = lastMousePos.y;
				lastMousePos.x = LOWORD(lParam);
				lastMousePos.y = HIWORD(lParam);
				RECT invalidateBounds;
				invalidateBounds.top = min(lastMousePos.y, lastMouseClick.y) - abs(oldMouseY - lastMousePos.y) - 8;
				invalidateBounds.bottom = max(lastMousePos.y, lastMouseClick.y) + abs(oldMouseY - lastMousePos.y) + 8;
				invalidateBounds.left = min(lastMousePos.x, lastMouseClick.x) - abs(oldMouseX - lastMousePos.x) - 8;
				invalidateBounds.right = max(lastMousePos.x, lastMouseClick.x) + abs(oldMouseX - lastMousePos.x) + 8;
				InvalidateRect(&invalidateBounds, true);
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			lastMousePos.x = LOWORD(lParam);
			lastMousePos.y = HIWORD(lParam);
			objectToLabel = track.leftReleased(lastMousePos);
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
			if(objectToLabel)
			{
				objectToLabel->setBeingRelabeled(false);
				objectToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_DESTROY: //When the window is destroyed
		remove("junk.xml");
		PostQuitMessage(0);
		break;
	}
	return WndProcDefault(message, wParam, lParam);
}
