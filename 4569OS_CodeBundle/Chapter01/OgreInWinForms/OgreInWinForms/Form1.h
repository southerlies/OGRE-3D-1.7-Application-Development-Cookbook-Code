#pragma once

#include "Engine.h"

namespace OgreInWinForms
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class OgreForm : public System::Windows::Forms::Form
	{


	public:
		OgreForm(void)
			: m_Engine(NULL)
		{
			InitializeComponent();
			m_Engine = new CEngine((HWND)this->Handle.ToPointer());
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~OgreForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// OgreForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 264);
			this->Name = L"OgreForm";
			this->Text = L"Ogre in WinForms";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Activated += gcnew System::EventHandler(this, &OgreForm::Ogre_Activated);
			this->Load += gcnew System::EventHandler(this, &OgreForm::Ogre_Load);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OgreForm::Ogre_Paint);
			this->Move += gcnew System::EventHandler(this, &OgreForm::OgreForm_Move);
			this->Resize += gcnew System::EventHandler(this, &OgreForm::OgreForm_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Ogre_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
			 {
				 	m_Engine->m_Root->renderOneFrame();
			 }
	private: System::Void Ogre_Load(System::Object^  sender, System::EventArgs^  e)
			 {
			 }
	private: System::Void Ogre_Activated(System::Object^  sender, System::EventArgs^  e) 
			 {
			 }
	public:



	private: System::Void OgreForm_Move(System::Object^  sender, System::EventArgs^  e) 
			 {
	
			 }
	public:
		CEngine *m_Engine;
	private: System::Void OgreForm_Resize(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if (m_Engine != NULL)
				 {
					 m_Engine->m_Root->renderOneFrame();
				 }
			 }
	};
}

