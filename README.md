# CopyTrack
CopyTrack helps you to track the location (file path, link, etc.) where you copy.

Imagine you copy some text from a browser page and paste it to your note application. If you want to record where the text is copied, you have to copy the web address and paste it aside the text.

Indeed, the system clipboard usually records such clues. For example, if you copy from a Chrome page, the system clipboard has an item "HTML Format" which contains the URL of the page. 

<img src="https://raw.githubusercontent.com/Megre/Media/main/image-20240804150003045.png" alt="image-20240804150003045" style="zoom: 80%;" />

<center>↑ The output of CopyTrack (UTF-8) when you search "Github" on bing.com and copy some text</center>

CopyTrack extracts such clues underneath the system clipboard, including web page address and Office Word file location. Some information is application specific since different applications use the system clipboard in different ways. 

## Applications

### Quicker

Typically, the output of CopyTrack can be used combined with automation software like [Quicker](getquicker.net) to collect fragmentation information you are browsing, such as the title of the web page, the location of the file, and the process name of the application, etc.

### Obsidian

If you collect Fragmented information using [Obsidian](https://obsidian.md/), the Local REST API plugin allows you add notes to Obsidian by putting HTTP request. The workflow of quickly making note can be automated with the help of Quicker:

1. Copy some text or image in notepad, Word, or web browser

2. Run CopyTrack and extract the output. The following clipboard formats are supported:

   TEXT, UNICODE TEXT, HTML Format (SourceURL, HTML fragment), LinkSource (WPS)

3. Making a note by posting the following information to https://127.0.0.1:27124/vault/folder/`{Text}`.md

   ```c#
   {HTMLFormat.HTML}
   > from [Link]({HTMLFormat.SourceURL})
   ```

   
