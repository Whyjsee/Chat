#include "../include/ChatTabUI.h"
#include "../include/Main.h"
#include <wx/image.h>


//#include "../capture.xpm"

ChatTabUI::ChatTabUI(wxNotebook* parentUI)
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    this->parentUI = parentUI;
}

void ChatTabUI::CreateTabUI(wxString& chatText,wxObject* mainFrame)
{

    panel = new wxPanel(parentUI);
    closeBtn = new wxButton(panel,wxID_CANCEL,_("关闭"));
    sendBtn = new wxButton(panel,wxID_OK,_("发送"));
    topSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    chatTextctrl = new wxRichTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,
                              wxSize(parentUI->GetClientSize().GetWidth(),parentUI->GetClientSize().GetHeight()*0.55),wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY |wxTE_AUTO_URL);
    chatTextctrl->AppendText(chatText);
    sendchatTextctrl = new wxRichTextCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,
                                  wxSize(parentUI->GetClientSize().GetWidth(),parentUI->GetClientSize().GetHeight()*0.2),wxTE_MULTILINE | wxTE_RICH | wxTE_AUTO_URL);
    topSizer->Add(chatTextctrl,0,wxEXPAND | wxALL,10);
    topSizer->Add(sendchatTextctrl,0,wxEXPAND | wxALL,10);
    wxGridSizer* toolsSizer = new wxGridSizer(3,3,10);
    wxBitmap icon(_("capture.png"),wxBITMAP_TYPE_PNG);

    wxBitmapButton* captureBtn =new wxBitmapButton(panel,ID_CAPTURE_IMAGE,icon,wxDefaultPosition,wxSize(40,40),wxBU_AUTODRAW);
    toolsSizer->Add(captureBtn,0,wxALIGN_CENTER,0);

    buttonSizer->Add(toolsSizer,0,wxALL,10);
    buttonSizer->Add(closeBtn,0, wxALL,10);
    buttonSizer->Add(sendBtn,0, wxALL,10);
    //topSizer.Add(uttonSizer,0,wxALIGN_CENTER,0);
    ((MainFrame*)mainFrame)->Connect(ID_CAPTURE_IMAGE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnCaptureImageBtnClk);
    ((MainFrame*)mainFrame)->Connect(wxID_CANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnCloseBtnClk);
    ((MainFrame*)mainFrame)->Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MainFrame::OnSendBtnClk);
    topSizer->Add(buttonSizer);
    panel->SetSizer(topSizer);
}

void ChatTabUI::AppendChatText(wxString& text)
{
    //chatTextctrl->AppendText(text);
   // chatTextctrl->GetBuffer().Copy(sendchatTextctrl->GetBuffer());
    //chatTextctrl->GetBuffer() = sendchatTextctrl->GetBuffer();
		chatTextctrl->Newline();
	wxRichTextBuffer& buffer = chatTextctrl->GetBuffer();
	
	//buffer.InsertNewlineWithUndo(chatTextctrl->GetLastPosition(),sendchatTextctrl,wxRICHTEXT_INSERT_INTERACTIVE);
	wxRichTextObjectList& chatObjectList = buffer.GetChildren();
	wxRichTextObjectList& sendObjectList = sendchatTextctrl->GetBuffer().GetChildren();
	
	wxRichTextObjectList::compatibility_iterator node = sendObjectList.GetFirst();
	while(node)
	{
	    wxRichTextObject* child = node->GetData();
        wxRichTextObject* newChild = child->Clone();
        newChild->SetParent(&buffer);
		chatObjectList.Append(newChild);
		node = node->GetNext();
	}
	buffer.UpdateRanges();
	chatTextctrl->LayoutContent();
    chatTextctrl->GetBuffer().Invalidate();
	chatTextctrl->RecreateBuffer();
    chatTextctrl->Refresh();
	
	chatTextctrl->ShowPosition(chatTextctrl->GetLastPosition());
}

wxString ChatTabUI::GetSendText()
{
    wxString text = sendchatTextctrl->GetValue();

    //sendchatTextctrl->GetPh

   // sendchatTextctrl->SetValue("");
	//sendchatTextctrl->SetFocus();
    return text;
}
void ChatTabUI::AppendImageToSendTextctrl(wxString& path)
{
    wxImage image(path,wxBITMAP_TYPE_PNG);
    if(image.Ok())
    {
        sendchatTextctrl->WriteImage(image);

    }
}
