import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PornhubVideoDownloader {
    public static void main(String[] args) {
        //String videoUrl = "https://www.pornhub.com/view_video.php?viewkey=ph5f7b8c9d1e2f3"; // Replace with your video URL
        //String videoUrl = "https://www.pornhub.com/view_video.php?viewkey=657d14ba797d3";
        String videoUrl ="https://www.pornhub.com/view_video.php?viewkey=640b34d82f61c";
        String outputDir = "downloaded_video"; // Where files will save
        String finalOutput = "output.mp4"; // Final combined file

        try {
            // Step 1: Create output directory
            File dir = new File(outputDir);
            if (!dir.exists()) dir.mkdirs();

            // Step 2: Fetch the .m3u8 playlist URL from the video page
            String m3u8Url = extractM3u8Url(videoUrl);
            if (m3u8Url == null) {
                System.out.println("Couldn’t find .m3u8 URL. Check the video URL or page structure.");
                return;
            }
            System.out.println("Found .m3u8 URL: " + m3u8Url);

            // Step 3: Get list of .ts segment URLs from the playlist
            List<String> segmentUrls = parseM3u8Playlist(m3u8Url);
            if (segmentUrls.isEmpty()) {
                System.out.println("No segments found in playlist.");
                return;
            }
            System.out.println("Found " + segmentUrls.size() + " segments.");

            // Step 4: Download each segment
            List<File> segmentFiles = downloadSegments(segmentUrls, outputDir);

            // Step 5: Combine segments into one file
            combineSegments(segmentFiles, outputDir + "/" + finalOutput);
            System.out.println("Video downloaded and combined as " + finalOutput);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Fetch HTML and extract .m3u8 URL (simplified, may need adjustment)
    private static String extractM3u8Url(String videoUrl) throws IOException {
        URL url = new URL(videoUrl);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestProperty("User-Agent", "Mozilla/5.0");
        BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
        String line;
        StringBuilder html = new StringBuilder();
        while ((line = reader.readLine()) != null) {
            html.append(line);
        }
        reader.close();
        conn.disconnect();

        // Look for .m3u8 in script tags (Pornhub often embeds it in JS)
        Pattern pattern = Pattern.compile("https?://[\\w/.-]+\\.m3u8");
        Matcher matcher = pattern.matcher(html.toString());
        if (matcher.find()) {
            return matcher.group();
        }
        return null; // If not found, might need browser dev tools to locate exact URL
    }

    // Parse .m3u8 file to get .ts segment URLs
    private static List<String> parseM3u8Playlist(String m3u8Url) throws IOException {
        List<String> segmentUrls = new ArrayList<>();
        URL url = new URL(m3u8Url);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestProperty("User-Agent", "Mozilla/5.0");
        BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
        String line;
        String baseUrl = m3u8Url.substring(0, m3u8Url.lastIndexOf("/") + 1);

        while ((line = reader.readLine()) != null) {
            if (line.endsWith(".ts")) {
                // Handle relative URLs
                if (!line.startsWith("http")) {
                    segmentUrls.add(baseUrl + line);
                } else {
                    segmentUrls.add(line);
                }
            }
        }
        reader.close();
        conn.disconnect();
        return segmentUrls;
    }

    // Download all .ts segments
    private static List<File> downloadSegments(List<String> segmentUrls, String outputDir) throws IOException {
        List<File> files = new ArrayList<>();
        int count = 0;
        for (String segmentUrl : segmentUrls) {
            String fileName = outputDir + "/segment_" + String.format("%03d", count++) + ".ts";
            File file = new File(fileName);
            downloadFile(segmentUrl, file);
            files.add(file);
            System.out.println("Downloaded: " + fileName);
        }
        return files;
    }

    // Download a single file from URL
    private static void downloadFile(String fileUrl, File outputFile) throws IOException {
        URL url = new URL(fileUrl);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestProperty("User-Agent", "Mozilla/5.0");
        try (InputStream in = conn.getInputStream();
             FileOutputStream out = new FileOutputStream(outputFile)) {
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = in.read(buffer)) != -1) {
                out.write(buffer, 0, bytesRead);
            }
        } finally {
            conn.disconnect();
        }
    }

    // Combine .ts files into one .mp4 file
    private static void combineSegments(List<File> segmentFiles, String outputPath) throws IOException {
        try (FileOutputStream out = new FileOutputStream(outputPath)) {
            for (File segment : segmentFiles) {
                try (FileInputStream in = new FileInputStream(segment)) {
                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    while ((bytesRead = in.read(buffer)) != -1) {
                        out.write(buffer, 0, bytesRead);
                    }
                }
            }
        }
    }
}