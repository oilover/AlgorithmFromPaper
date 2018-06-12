using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Outlook = Microsoft.Office.Interop.Outlook;
using Office = Microsoft.Office.Core;
using System.IO;
using Microsoft.Office.Interop.Outlook;
using Word = Microsoft.Office.Interop.Word;
using Microsoft.Office.Tools.Ribbon;
using System.Windows.Forms;

namespace EmailStream
{
    public partial class ThisAddIn
    {
        internal static Outlook.Application app;//= new Outlook.Application();
        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {           
            app = this.Application;
       

            
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
            // Note: Outlook no longer raises this event. If you have code that 
            //    must run when Outlook shuts down, see https://go.microsoft.com/fwlink/?LinkId=506785
        }
        static string gao(string illegal)
        {
           // string illegal = "\"M\"\\a/ry/ h**ad:>> a\\/:*?\"| li*tt|le|| la\"mb.?";
            string invalid = new string(Path.GetInvalidFileNameChars()) + new string(Path.GetInvalidPathChars());

            foreach (char c in invalid)
            {
                illegal = illegal.Replace(c.ToString(), ""); 
            }
            return illegal;
        }
        
    public static void WriteEmail()
        {
            
            string fileName = "";
            MailItem mail = null;
            try
            {
                Outlook.Application app = ThisAddIn.app;
                NameSpace sess = app.Session;
                var folders = app.Session.Folders;
                MAPIFolder inbox; //= sess.GetDefaultFolder(OlDefaultFolders.olFolderInbox);
                inbox = sess.PickFolder();
                Items items = inbox.Items;
                FolderBrowserDialog sfg = new FolderBrowserDialog();
                
                sfg.ShowDialog(); string dir = sfg.SelectedPath; MessageBox.Show(dir);
            //    using (StreamWriter sw = new StreamWriter(sfg.OpenFile())) // (inbox.Name + @"__email_record.txt", true))
                {
                    int n = items.Count;
                    for (int i = 1; i <= n; i++)
                    {
                        var it = items[i];
                        mail = it as MailItem;
                        
                        if (mail != null)
                        {
                            fileName = mail.Subject;
                            fileName = gao(fileName);
                            fileName = fileName.Substring(0, Math.Min(fileName.Length, 100));
                            fileName = dir + "\\" + fileName + i + ".txt";
                            MessageBox.Show(@fileName);
                            using (StreamWriter writer = new StreamWriter(@fileName))
                            {
                                writer.WriteLine(fileName);
                                writer.WriteLine("Subject:" + mail.Subject);
                                writer.WriteLine("TO:" + mail.To);
                                writer.WriteLine("==========================");
                                writer.WriteLine(mail.CC);
                                writer.WriteLine("==========================");
                                writer.WriteLine(mail.Body);
                            }
                                
                        }
                      
                    }
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(mail.Subject + "===>" + fileName);
                System.Windows.Forms.MessageBox.Show(ex.Message, ex.Message, System.Windows.Forms.MessageBoxButtons.OK);
            }
        }

        #region VSTO generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InternalStartup()
        {
            this.Startup += new System.EventHandler(ThisAddIn_Startup);
            this.Shutdown += new System.EventHandler(ThisAddIn_Shutdown);
        }
        
        #endregion
    }
}
