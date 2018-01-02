if ($env:APPVEYOR_JOB_NUMBER -ne 1)
{
  Write-Output "Restoring build cache from artifact..."

  $apiUrl = 'https://ci.appveyor.com/api'
  $headers = @{
    "Authorization" = "Bearer $env:appveyor_api_token"
    "Content-type" = "application/json"
  }
  $accountName = 'gmrukwa'
  $projectSlug = 'native-algorithms'

  # using clone folder...
  $downloadLocation = 'C:\projects\native-algorithms'

  # get last job details
  $project = Invoke-RestMethod -Method Get -Uri "$apiUrl/projects/$accountName/$projectSlug/build/$env:RAW_BUILD_VERSION" -Headers $headers

  # get this job id
  $jobId = $project.build.jobs[$env:APPVEYOR_JOB_NUMBER - 2].jobId

  # get job artifacts - an array of jobs
  $artifacts = Invoke-RestMethod -Method Get -Uri "$apiUrl/buildjobs/$jobId/artifacts" -Headers $headers

  # select the first [0] artifact, or  use the artifact name - e.g. $artifactFileName = 'MyPath/MyArtifact.zip'
  # $artifactFileName = $artifacts[0].fileName
  $artifactFileName = "cached_artifacts.zip"

  # artifact will be downloaded as
  $localArtifactArchive = "$downloadLocation\$artifactFileName"

  # download artifact
  Invoke-RestMethod -Method Get -Uri "$apiUrl/buildjobs/$jobId/artifacts/$artifactFileName" `
  -OutFile $localArtifactArchive -Headers @{ "Authorization" = "Bearer $token" }

  if (-not (Test-Path $localArtifactArchive)) {
    Write-Output "Error artifact $localArtifactArchive not found"
  } else {
    Write-Output "Extracting artifact $localArtifactArchive ..."
    & 7z.exe x -aos $localArtifactArchive | Select-String -Pattern "^Extracting " -CaseSensitive -NotMatch
    Write-Output "Build cache restored from artifact $artifactFileName"
  }
}
